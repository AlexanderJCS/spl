#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    Parser(std::vector<token::Token> input);

    [[nodiscard]] const ast::ASTNode& root() const;

private:
    ast::ASTNode parse();
    [[nodiscard]] bool atEnd() const;
    [[nodiscard]] token::Token currentToken() const;
    token::Token advance();
    ast::ASTNode parseStatement();
    ast::ASTNode parseDeclaration();
    ast::ASTNode parseExpression();
    void expect(token::TokenType type);

    ast::ASTNode astRoot;
    std::vector<token::Token> tokens;
    int pos;
};

#endif  // SPL_PARSER_H
