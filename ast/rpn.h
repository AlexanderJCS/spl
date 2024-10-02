#ifndef SPL_RPN_H
#define SPL_RPN_H

#include "ast.h"
#include "../token/tokenizer.h"

#include <string>
#include <vector>
#include <stack>


// TODO: put all this in a namespace

class ShuntingYardParser {
public:
    explicit ShuntingYardParser(std::vector<token::Token> input);

    [[nodiscard]] const ast::ASTNode& root() const;

private:
    [[nodiscard]] ast::ASTNode parse() const;

    static void addNode(std::stack<ast::ASTNode>& operandStack, const token::Token& token);

    std::vector<token::Token> tokens;
    ast::ASTNode astRoot;
};

#endif  // SPL_RPN_H
