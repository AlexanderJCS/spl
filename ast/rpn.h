#ifndef SPL_RPN_H
#define SPL_RPN_H

#include "../token/tokenizer.h"

#include <string>
#include <vector>
#include <stack>


// TODO: put all this in a namespace

class ASTNode {
public:
    ASTNode(const token::Token& value, std::vector<ASTNode> children);

    [[nodiscard]] const token::Token& token() const;
    [[nodiscard]] const std::vector<ASTNode>& children() const;

    [[nodiscard]] int line() const;
    [[nodiscard]] int column() const;

private:
    token::Token nodeToken;
    std::vector<ASTNode> nodeChildren;

    int lineAt;
    int columnAt;
};

class ShuntingYardParser {
public:
    explicit ShuntingYardParser(std::vector<token::Token> input);

    [[nodiscard]] const ASTNode& root() const;

private:
    [[nodiscard]] ASTNode parse() const;

    static void addNode(std::stack<ASTNode>& operandStack, const token::Token& token);

    std::vector<token::Token> tokens;
    ASTNode astRoot;
};

#endif  // SPL_RPN_H
