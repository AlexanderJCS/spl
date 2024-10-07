#include "rpn.h"

#include "ast.h"
#include "tokenizer.h"

#include <utility>
#include <stack>
#include <stdexcept>


ShuntingYardParser::ShuntingYardParser(std::vector<token::Token> input)
    : tokens(std::move(input)), astRoot(parse()) {}


ast::ExpressionNode ShuntingYardParser::parse() const {
    std::stack<token::Token> operatorStack;
    std::stack<ast::ExpressionNode> operandStack;

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

            // default case: the token is not an operator
            default:
                operandStack.push(ast::ExpressionNode{token, {}});
                break;
        }
    }

    while (!operatorStack.empty()) {
        addNode(operandStack, operatorStack.top());
        operatorStack.pop();
    }

    return operandStack.top();
}

const ast::ExpressionNode& ShuntingYardParser::root() const {
    return astRoot;
}

void ShuntingYardParser::addNode(std::stack<ast::ExpressionNode>& operandStack, const token::Token& token) {
    if (token.type() == token::TokenType::IDENTIFIER || token.type() == token::TokenType::LITERAL_INT) {
        operandStack.push(ast::ExpressionNode{token, {}});
    } if (token.type() == token::TokenType::FUNCTION_CALL) {
        // Assuming token is a pointer or reference to token::Token
        const auto* functionCallTokenPtr = dynamic_cast<const token::FunctionCallToken*>(&token);

        if (!functionCallTokenPtr) {
            throw std::runtime_error("Error: expected FunctionCallToken but got something else");
        }

        const auto& functionCallToken = *functionCallTokenPtr;

        // todo: this should probably be a method in the FunctionCallToken class, or directly in the FunctionCallNode
        //  constructor
        std::vector<std::shared_ptr<ast::ASTNode>> arguments;
        for (const ast::ExpressionNode& argument : functionCallToken.arguments()) {
            arguments.push_back(std::make_shared<ast::ExpressionNode>(argument));
        }

        operandStack.push(ast::FunctionCallNode(token, arguments));
    } else {
        ast::ExpressionNode right = operandStack.top();
        operandStack.pop();
        ast::ExpressionNode left = operandStack.top();
        operandStack.pop();

        std::shared_ptr<ast::ASTNode> leftPtr = std::make_shared<ast::ExpressionNode>(left);
        std::shared_ptr<ast::ASTNode> rightPtr = std::make_shared<ast::ExpressionNode>(right);

        operandStack.push(ast::ExpressionNode{token, {leftPtr, rightPtr}});
    }
}
