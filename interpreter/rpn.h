#ifndef SPL_RPN_H
#define SPL_RPN_H

#include "ast.h"
#include "tokenizer.h"

#include <string>
#include <vector>
#include <stack>


// TODO: put all this in a namespace

class ShuntingYardParser {
public:
    explicit ShuntingYardParser(std::vector<token::Token> input);

    [[nodiscard]] const ast::ExpressionNode& root() const;

private:
    [[nodiscard]] ast::ExpressionNode parse() const;

    static void addNode(std::stack<ast::ExpressionNode>& operandStack, const token::Token& token);

    std::vector<token::Token> tokens;
    ast::ExpressionNode astRoot;
};

#endif  // SPL_RPN_H
