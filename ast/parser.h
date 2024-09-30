#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    Parser(std::vector<token::Token> input);

    [[nodiscard]] const node::ASTNode& root() const;

private:
    node::ASTNode parse();
    [[nodiscard]] bool atEnd() const;
    [[nodiscard]] token::Token currentToken() const;
    token::Token advance();
    node::ASTNode parseStatement();
    node::ASTNode parseDeclaration();
    node::ASTNode parseExpression();
    void expect(token::TokenType type);

    node::ASTNode astRoot;
    std::vector<token::Token> tokens;
    int pos;
};

#endif  // SPL_PARSER_H
