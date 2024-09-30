#include "rpn.h"

#include "ast.h"
#include "../token/tokenizer.h"

#include <utility>
#include <stack>


ShuntingYardParser::ShuntingYardParser(std::vector<token::Token> input)
    : tokens(std::move(input)), astRoot(parse()) {}


node::ASTNode ShuntingYardParser::parse() const {
    std::stack<token::Token> operatorStack;
    std::stack<node::ASTNode> operandStack;

    for (const token::Token& token : tokens) {
        switch (token.type()) {
            case token::TokenType::IDENTIFIER:
            case token::TokenType::LITERAL_INT:
                addNode(operandStack, token);
                break;

            case token::TokenType::SEMICOLON:
                // clear the operator stack
                while (!operatorStack.empty()) {
                    addNode(operandStack, operatorStack.top());
                    operatorStack.pop();
                }
                break;

            case token::TokenType::OPEN_PAREN:
                operatorStack.push(token);
                break;

            case token::TokenType::CLOSE_PAREN:
                while (!operatorStack.empty() && operatorStack.top().type() != token::TokenType::OPEN_PAREN) {
                    addNode(operandStack, operatorStack.top());
                    operatorStack.pop();
                }

                operatorStack.pop();  // pop closing parenthesis
                break;

            // add all operators to this case statement
            case token::TokenType::OPERATOR_DEFINE:
                while (!operatorStack.empty() &&  // stack is not empty
                       (operatorStack.top().precedence() > token.precedence() ||  // higher precedence
                       (operatorStack.top().precedence() == token.precedence() && token.associativity() == token::Associativity::LEFT))  // equal precedence if left associative
                       )
                {
                    addNode(operandStack, operatorStack.top());
                    operatorStack.pop();
                }

                operatorStack.push(token);
                break;

            // default case: the token is not an operator
            default:
                operandStack.push(node::ASTNode{node::NodeType::EXPRESSION, token, {}});
                break;
        }
    }

    while (!operatorStack.empty()) {
        addNode(operandStack, operatorStack.top());
        operatorStack.pop();
    }

    return operandStack.top();
}


const node::ASTNode &ShuntingYardParser::root() const {
    return astRoot;
}


void ShuntingYardParser::addNode(std::stack<node::ASTNode> &operandStack, const token::Token &token) {
    if (token.type() == token::TokenType::IDENTIFIER || token.type() == token::TokenType::LITERAL_INT) {
        operandStack.push(node::ASTNode{node::NodeType::EXPRESSION, token, {}});
    } else {
        node::ASTNode right = operandStack.top();
        operandStack.pop();
        node::ASTNode left = operandStack.top();
        operandStack.pop();

        operandStack.push(node::ASTNode{node::NodeType::EXPRESSION, token, {left, right}});
    }
}
