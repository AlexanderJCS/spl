#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    Parser(std::vector<token::Token> input);

    [[nodiscard]] const node::ASTNode& root() const;

private:
    node::ASTNode parse();
    bool atEnd();
    token::Token currentToken();
    token::Token advance();
    node::ASTNode parseStatement();
    node::ASTNode parseDeclaration();
    node::ASTNode parseExpression();

    node::ASTNode astRoot;
    std::vector<token::Token> tokens;
    int pos;
};

#endif  // SPL_PARSER_H
