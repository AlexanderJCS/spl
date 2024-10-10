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
    explicit ShuntingYardParser(std::vector<std::shared_ptr<token::Token>> input);

    [[nodiscard]] std::shared_ptr<ast::ExpressionNode> root() const;

private:
    [[nodiscard]] std::shared_ptr<ast::ExpressionNode> parse() const;

    static void addNode(std::stack<std::shared_ptr<ast::ExpressionNode>>& operandStack, const token::Token& token);

    std::vector<std::shared_ptr<token::Token>> tokens;
    std::shared_ptr<ast::ExpressionNode> astRoot;
};

#endif  // SPL_RPN_H
