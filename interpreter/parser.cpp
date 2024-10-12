#include "parser.h"

#include "rpn.h"

#include <stdexcept>


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
    }

    expect(token::TokenType::CLOSE_PAREN);

    return token::FunctionCallToken{identifier.value(), identifier.line(), identifier.column(), arguments};
}


std::shared_ptr<ast::ExpressionNode> Parser::parseExpression() {
    std::vector<std::shared_ptr<token::Token>> expressionTokens;

    while (currentToken().type() != token::TokenType::SEMICOLON && currentToken().type() != token::TokenType::SEPARATOR) {
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
    expect(token::TokenType::OPEN_BRACE);

    // get all tokens within the curly braces and create a parser for them
    std::vector<token::Token> functionTokens;
    int braceCount = 1;

    while (braceCount > 0) {
        token::Token token = advance();

        if (token.type() == token::TokenType::OPEN_BRACE) {
            braceCount++;
        } else if (token.type() == token::TokenType::CLOSE_BRACE) {
            braceCount--;
        }

        if (braceCount > 0) {
            functionTokens.push_back(token);
        }
    }

    Parser functionParser{functionTokens};

    return std::make_shared<ast::FunctionDefNode>(ast::FunctionDefNode{
        identifier,
        arguments,
        std::static_pointer_cast<ast::ASTNode>(std::make_shared<ast::RootNode>(functionParser.root()))
    });
}
