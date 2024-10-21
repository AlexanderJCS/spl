#include "parser.h"

#include "rpn.h"

#include <stdexcept>
#include <iostream>


Parser::Parser(std::vector<token::Token> input) {
    tokens = std::move(input);
    pos = 0;

    astRoot = parse();
}


ast::RootNode Parser::parse() {
    ast::RootNode root;

    while (!atEnd()) {
        std::shared_ptr<ast::ASTNode> statement = parseStatement();

        if (statement != nullptr) {
            root.children().push_back(statement);
        }
    }

    return root;
}


ast::RootNode Parser::root() const {
    return astRoot;
}


bool Parser::atEnd() const {
    return pos >= tokens.size();
}


token::Token Parser::currentToken() const {
    return tokens[pos];
}


token::Token Parser::advance() {
    return tokens[pos++];
}


token::Token Parser::peek() const {
    if (pos + 1 >= tokens.size()) {
        throw std::runtime_error("Could not peek: end of input");
    }

    return tokens[pos + 1];
}


std::shared_ptr<ast::ASTNode> Parser::parseStatement() {
    if (currentToken().type() == token::TokenType::SEMICOLON) {
        advance();  // skip
    } else if (currentToken().type() == token::TokenType::IDENTIFIER && peek().type() == token::TokenType::OPERATOR_DEFINE) {
        std::shared_ptr<ast::DeclarationNode> declaration = parseDeclaration();
        std::shared_ptr<ast::ASTNode> declarationNode = std::static_pointer_cast<ast::ASTNode>(declaration);

        return declarationNode;
    } else if (currentToken().type() == token::TokenType::IDENTIFIER || currentToken().type() == token::TokenType::LITERAL_INT) {
        std::shared_ptr<ast::ExpressionNode> expression = parseExpression();
        std::shared_ptr<ast::ASTNode> expressionNode = std::static_pointer_cast<ast::ASTNode>(expression);

        return expressionNode;
    } else if (currentToken().type() == token::TokenType::FUNCTION_DEF) {
        std::shared_ptr<ast::FunctionDefNode> functionDef = parseFuncDeclaration();
        std::shared_ptr<ast::ASTNode> functionDefNode = std::static_pointer_cast<ast::ASTNode>(functionDef);

        return functionDefNode;
    } else if (currentToken().type() == token::TokenType::RETURN) {
        std::shared_ptr<ast::ControlFlowNode> controlFlow = parseJump();
        std::shared_ptr<ast::ASTNode> controlFlowNode = std::static_pointer_cast<ast::ASTNode>(controlFlow);

        return controlFlowNode;
    } else if (currentToken().type() == token::TokenType::IF_STATEMENT) {
        std::shared_ptr<ast::IfNode> ifStatement = parseIf();
        std::shared_ptr<ast::ASTNode> ifStatementNode = std::static_pointer_cast<ast::ASTNode>(ifStatement);

        return ifStatementNode;
    } else {
        throw std::runtime_error("Could not determine how to parse a statement from the current token");
    }

    return nullptr;
}


std::shared_ptr<ast::DeclarationNode> Parser::parseDeclaration() {
    token::Token identifier = advance();
    token::Token nextToken = advance();

    ast::DeclarationNode declaration{
        {
            std::make_shared<ast::ExpressionNode>(ast::ExpressionNode{identifier, {}})
        }
    };

    if (nextToken.type() == token::TokenType::OPERATOR_DEFINE) {
        std::shared_ptr<ast::ExpressionNode> expression = parseExpression();
        std::shared_ptr<ast::ASTNode> expressionPtr = std::static_pointer_cast<ast::ASTNode>(expression);

        declaration.children().push_back(expressionPtr);
    }

    expect(token::TokenType::SEMICOLON);
    return std::make_shared<ast::DeclarationNode>(declaration);
}


token::FunctionCallToken Parser::parseFunctionCall() {
    token::Token identifier = advance();
    expect(token::TokenType::OPEN_PAREN);

    std::vector<std::shared_ptr<ast::ExpressionNode>> arguments;

    while (currentToken().type() != token::TokenType::CLOSE_PAREN) {
        arguments.push_back(parseExpression());

        // supports syntax like funcCall(arg1) instead of funcCall(arg1,)
        if (currentToken().type() == token::TokenType::CLOSE_PAREN) {
            break;
        }

        advance();  // skip the comma
    }

    expect(token::TokenType::CLOSE_PAREN);

    return token::FunctionCallToken{identifier.value(), identifier.line(), identifier.column(), arguments};
}


std::shared_ptr<ast::ExpressionNode> Parser::parseExpression() {
    std::vector<std::shared_ptr<token::Token>> expressionTokens;

    int parenCount = 0;
    while (currentToken().type() != token::TokenType::SEMICOLON && currentToken().type() != token::TokenType::SEPARATOR) {
        if (currentToken().type() == token::TokenType::OPEN_PAREN) {
            parenCount++;
        } else if (currentToken().type() == token::TokenType::CLOSE_PAREN) {
            parenCount--;
        }

        if (parenCount < 0) {
            break;
        }

        if (!atEnd() && currentToken().type() == token::TokenType::IDENTIFIER && peek().type() == token::TokenType::OPEN_PAREN) {
            token::FunctionCallToken functionCall = parseFunctionCall();
            expressionTokens.push_back(std::make_shared<token::FunctionCallToken>(functionCall));
        } else {
            expressionTokens.push_back(std::make_shared<token::Token>(advance()));
        }
    }

    ShuntingYardParser shuntingYardParser{expressionTokens};

    return shuntingYardParser.root();
}


void Parser::expect(token::TokenType type) {
    if (advance().type() != type) {
        throw std::runtime_error("Parser expected one token type, but got another");
    }
}

std::shared_ptr<ast::FunctionDefNode> Parser::parseFuncDeclaration() {
    advance();  // skip the function keyword
    token::Token identifier = advance();
    expect(token::TokenType::OPEN_PAREN);

    std::vector<std::string> arguments;

    while (currentToken().type() != token::TokenType::CLOSE_PAREN) {
        switch (currentToken().type()) {
            case token::TokenType::IDENTIFIER:
                arguments.push_back(currentToken().value());
                break;
            case token::TokenType::SEPARATOR:
                break;  // do nothing
            default:
                throw std::runtime_error("Unexpected token in function declaration");
        }

        advance();
    }

    expect(token::TokenType::CLOSE_PAREN);  // should be true because of the while loop

    // get all tokens within the curly braces and create a parser for them
    std::vector<token::Token> functionTokens = extractEnclosedTokens(token::TokenType::OPEN_BRACE, token::TokenType::CLOSE_BRACE);

    Parser functionParser{functionTokens};

    return std::make_shared<ast::FunctionDefNode>(ast::FunctionDefNode{
        identifier,
        arguments,
        std::static_pointer_cast<ast::ASTNode>(std::make_shared<ast::RootNode>(functionParser.root()))
    });
}

std::shared_ptr<ast::ControlFlowNode> Parser::parseJump() {
    token::Token jumpToken = advance();

    switch (jumpToken.type()) {
        case token::TokenType::RETURN: {
            std::shared_ptr<ast::ExpressionNode> expression = parseExpression();
            std::shared_ptr<ast::ASTNode> expressionNode = std::static_pointer_cast<ast::ASTNode>(expression);

            return std::make_shared<ast::ControlFlowNode>(ast::ControlFlowNode{jumpToken, {expressionNode}});
        }
        default:
            throw std::runtime_error("Unexpected control flow token");
    }
}

std::shared_ptr<ast::IfNode> Parser::parseIf() {
    token::Token ifToken = advance();  // skip the "if" keyword

    std::vector<std::shared_ptr<ast::ASTNode>> children;

    // parse the 'if' condition and body
    std::vector<std::shared_ptr<token::Token>> ifConditionTokens = extractEnclosedTokensPtr(token::TokenType::OPEN_PAREN, token::TokenType::CLOSE_PAREN);
    std::vector<token::Token> ifBodyTokens = extractEnclosedTokens(token::TokenType::OPEN_BRACE, token::TokenType::CLOSE_BRACE);

    ShuntingYardParser ifConditionParser{ifConditionTokens};
    Parser ifBodyParser{ifBodyTokens};

    children.push_back(ifConditionParser.root());
    children.push_back(std::make_shared<ast::RootNode>(ifBodyParser.root()));

    // parse elif statements
    while (!atEnd() && currentToken().type() == token::TokenType::ELIF_STATEMENT) {
        advance();  // skip the "elif" keyword

        std::vector<std::shared_ptr<token::Token>> elifConditionTokens = extractEnclosedTokensPtr(token::TokenType::OPEN_PAREN, token::TokenType::CLOSE_PAREN);
        std::vector<token::Token> elifBodyTokens = extractEnclosedTokens(token::TokenType::OPEN_BRACE, token::TokenType::CLOSE_BRACE);

        ShuntingYardParser elifConditionParser{elifConditionTokens};
        Parser elifBodyParser{elifBodyTokens};

        children.push_back(elifConditionParser.root());
        children.push_back(std::make_shared<ast::RootNode>(elifBodyParser.root()));
    }

    // parse else statement
    if (!atEnd() && currentToken().type() == token::TokenType::ELSE_STATEMENT) {
        advance();  // skip the "else" keyword

        std::vector<token::Token> elseBodyTokens = extractEnclosedTokens(token::TokenType::OPEN_BRACE, token::TokenType::CLOSE_BRACE);
        Parser elseBodyParser{elseBodyTokens};

        children.push_back(std::make_shared<ast::RootNode>(elseBodyParser.root()));
    }

    return std::make_shared<ast::IfNode>(ast::IfNode{ifToken, children});
}

std::vector<std::shared_ptr<token::Token>> Parser::extractEnclosedTokensPtr(token::TokenType start, token::TokenType end) {
    expect(start);

    std::vector<std::shared_ptr<token::Token>> enclosedTokens;
    int count = 1;

    while (count > 0) {
        token::Token token = advance();

        if (token.type() == start) {
            count++;
        } else if (token.type() == end) {
            count--;
        }

        if (count > 0) {
            enclosedTokens.push_back(std::make_shared<token::Token>(token));
        }
    }

    return enclosedTokens;
}

std::vector<token::Token> Parser::extractEnclosedTokens(token::TokenType start, token::TokenType end) {
    std::vector<std::shared_ptr<token::Token>> enclosedTokensPtr = extractEnclosedTokensPtr(start, end);

    std::vector<token::Token> enclosedTokens;
    for (const std::shared_ptr<token::Token>& tokenPtr : enclosedTokensPtr) {
        enclosedTokens.push_back(*tokenPtr);
    }

    return enclosedTokens;
}
