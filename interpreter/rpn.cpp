#include "rpn.h"

#include "ast.h"
#include "tokenizer.h"

#include <utility>
#include <stack>
#include <stdexcept>
#include <iostream>


ShuntingYardParser::ShuntingYardParser(std::vector<std::shared_ptr<token::Token>> input)
    : tokens(std::move(input)), astRoot(parse()) {}


std::shared_ptr<ast::ExpressionNode> ShuntingYardParser::parse() const {
    std::stack<token::Token> operatorStack;
    std::stack<std::shared_ptr<ast::ExpressionNode>> operandStack;

    for (const std::shared_ptr<token::Token>& tokenPtr : tokens) {
        token::Token token = *tokenPtr;

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
            case token::TokenType::OPERATOR_ADD:
            case token::TokenType::OPERATOR_SUB:
            case token::TokenType::OPERATOR_MUL:
            case token::TokenType::OPERATOR_DIV:
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

            case token::TokenType::FUNCTION_CALL: {
                std::shared_ptr<token::FunctionCallToken> functionCallTokenPtr = std::dynamic_pointer_cast<token::FunctionCallToken>(tokenPtr);

                if (!functionCallTokenPtr) {
                    throw std::runtime_error("Error: expected FunctionCallToken but got something else");
                }

                const token::FunctionCallToken& functionCallToken = *functionCallTokenPtr;

                operandStack.push(std::make_shared<ast::FunctionCallNode>(ast::FunctionCallNode{functionCallToken}));
                break;
            }

            // default case: the token is not an operator
            default:
                operandStack.push(std::make_shared<ast::ExpressionNode>(ast::ExpressionNode{token, {}}));
                break;
        }
    }

    while (!operatorStack.empty()) {
        addNode(operandStack, operatorStack.top());
        operatorStack.pop();
    }

    return operandStack.top();
}

std::shared_ptr<ast::ExpressionNode> ShuntingYardParser::root() const {
    return astRoot;
}

void ShuntingYardParser::addNode(std::stack<std::shared_ptr<ast::ExpressionNode>>& operandStack, const token::Token& token) {
    if (token.type() == token::TokenType::IDENTIFIER || token.type() == token::TokenType::LITERAL_INT) {
        operandStack.push(std::make_shared<ast::ExpressionNode>(ast::ExpressionNode{token, {}}));
    } if (token.type() == token::TokenType::FUNCTION_CALL) {
        // Assuming token is a pointer or reference to token::Token
        const auto* functionCallTokenPtr = dynamic_cast<const token::FunctionCallToken*>(&token);

        if (!functionCallTokenPtr) {
            throw std::runtime_error("Error: expected FunctionCallToken but got something else");
        }

        const auto& functionCallToken = *functionCallTokenPtr;

        operandStack.push(std::make_shared<ast::FunctionCallNode>(ast::FunctionCallNode{functionCallToken}));
    } else {
        std::shared_ptr<ast::ExpressionNode> right = operandStack.top();
        operandStack.pop();
        std::shared_ptr<ast::ExpressionNode> left = operandStack.top();
        operandStack.pop();

        std::shared_ptr<ast::ASTNode> leftPtr = std::static_pointer_cast<ast::ASTNode>(left);
        std::shared_ptr<ast::ASTNode> rightPtr = std::static_pointer_cast<ast::ASTNode>(right);

        operandStack.push(std::make_shared<ast::ExpressionNode>(ast::ExpressionNode{token, {leftPtr, rightPtr}}));
    }
}
