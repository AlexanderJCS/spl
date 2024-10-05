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
        root.children().push_back(parseStatement());
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
    return tokens[pos + 1];
}


std::shared_ptr<ast::ASTNode> Parser::parseStatement() {
    if (currentToken().type() == token::TokenType::IDENTIFIER && peek().type() == token::TokenType::OPERATOR_DEFINE) {
        ast::DeclarationNode declaration = parseDeclaration();
        std::shared_ptr<ast::ASTNode> declarationNode = std::make_shared<ast::DeclarationNode>(declaration);

        return declarationNode;

    } else if (currentToken().type() == token::TokenType::IDENTIFIER || currentToken().type() == token::TokenType::LITERAL_INT) {
        ast::ExpressionNode expression = parseExpression();
        std::shared_ptr<ast::ASTNode> expressionNode = std::make_shared<ast::ExpressionNode>(expression);

        return expressionNode;
    } else {
        throw std::runtime_error("Unexpected token");
    }
}


ast::DeclarationNode Parser::parseDeclaration() {
    token::Token identifier = advance();
    token::Token nextToken = advance();

    ast::DeclarationNode declaration{
        {
            std::make_shared<ast::ExpressionNode>(ast::ExpressionNode{identifier, {}})
        }
    };

    if (nextToken.type() == token::TokenType::OPERATOR_DEFINE) {
        ast::ExpressionNode expression = parseExpression();
        std::shared_ptr<ast::ASTNode> expressionPtr = std::make_shared<ast::ExpressionNode>(expression);

        declaration.children().push_back(expressionPtr);
    }

    expect(token::TokenType::SEMICOLON);
    return declaration;
}


ast::ExpressionNode Parser::parseExpression() {
    std::vector<token::Token> expressionTokens;

    while (currentToken().type() != token::TokenType::SEMICOLON) {
        expressionTokens.push_back(advance());
    }

    ShuntingYardParser shuntingYardParser{expressionTokens};

    return shuntingYardParser.root();
}


void Parser::expect(token::TokenType type) {
    if (advance().type() != type) {
        throw std::runtime_error("Unexpected token");
    }
}
