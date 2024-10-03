#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    Parser(std::vector<token::Token> input);

    [[nodiscard]] ast::RootNode root() const;

private:
    ast::RootNode parse();
    [[nodiscard]] bool atEnd() const;
    [[nodiscard]] token::Token currentToken() const;
    token::Token advance();
    std::shared_ptr<ast::ASTNode> parseStatement();
    ast::DeclarationNode parseDeclaration();
    ast::ExpressionNode parseExpression();
    void expect(token::TokenType type);

    ast::RootNode astRoot;
    std::vector<token::Token> tokens;
    int pos;
};

#endif  // SPL_PARSER_H
