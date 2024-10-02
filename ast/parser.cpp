#include "parser.h"

#include "rpn.h"

#include <stdexcept>


Parser::Parser(std::vector<token::Token> input) {
    tokens = std::move(input);
    pos = 0;

    astRoot = parse();
}


ast::ASTNode Parser::parse() {
    ast::ASTNode root;

    while (!atEnd()) {
        root.childrenRef().push_back(parseDeclaration());
    }

    return root;
}


const ast::ASTNode& Parser::root() const {
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


ast::ASTNode Parser::parseStatement() {
    if (currentToken().type() == token::TokenType::TYPE_SPECIFIER_INT) {
        return parseDeclaration();
    } else if (currentToken().type() == token::TokenType::IDENTIFIER || currentToken().type() == token::TokenType::LITERAL_INT) {
        return parseExpression();
    } else {
        throw std::runtime_error("Unexpected token");
    }
}


ast::ASTNode Parser::parseDeclaration() {
    token::Token typeSpecifier = advance();
    token::Token identifier = advance();
    token::Token nextToken = advance();

    ast::ASTNode declaration = ast::ASTNode(ast::NodeType::DECLARATION, typeSpecifier, {ast::ASTNode(ast::NodeType::EXPRESSION, identifier, {})});

    if (nextToken.type() == token::TokenType::OPERATOR_DEFINE) {
        ast::ASTNode expression = parseExpression();

        declaration.childrenRef().push_back(expression);
    }

    expect(token::TokenType::SEMICOLON);
    return declaration;
}


ast::ASTNode Parser::parseExpression() {
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
