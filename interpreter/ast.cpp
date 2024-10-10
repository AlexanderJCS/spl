#include "ast.h"

#include <stdexcept>
#include <utility>
#include <memory>
#include <iostream>

const token::Token& ast::ASTNode::token() const {
    return nodeToken;
}

std::vector<std::shared_ptr<ast::ASTNode>>& ast::ASTNode::children() {
    return nodeChildren;
}

int ast::ASTNode::line() const {
    return lineAt;
}

int ast::ASTNode::column() const {
    return columnAt;
}

ast::ASTNode::ASTNode() : lineAt(-1), columnAt(-1) {}

ast::ASTNode::ASTNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children)
    : nodeToken(std::move(token)), nodeChildren(std::move(children)), lineAt(-1), columnAt(-1) {}

std::variant<int, float, std::string> ast::RootNode::eval(env::Environment &env) const {
    for (std::shared_ptr<ast::ASTNode> child : nodeChildren) {
        child->eval(env);
    }

    return {};
}

ast::RootNode::RootNode(const std::vector<std::shared_ptr<ASTNode>> &children) {
    nodeChildren = children;
}


std::variant<int, float, std::string> ast::DeclarationNode::eval(env::Environment &env) const {
    token::Token typeSpecifier = nodeToken;
    token::Token identifier = nodeChildren[0]->token();
    std::variant<int, float, std::string> value = nodeChildren[1]->eval(env);

    env.set(identifier.value(), value);

    return {};
}

ast::DeclarationNode::DeclarationNode(std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(token::Token(), std::move(children)) {}

std::variant<int, float, std::string> ast::StatementNode::eval(env::Environment& env) const {
    return -1;  // not implemented
}

ast::StatementNode::StatementNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(std::move(token), std::move(children)) {}

std::variant<int, float, std::string> ast::ExpressionNode::eval(env::Environment& env) const {
    if (nodeChildren.empty()) {
        if (nodeToken.type() == token::TokenType::IDENTIFIER) {
            return env.get(nodeToken.value());
        } else if (nodeToken.type() == token::TokenType::LITERAL_INT) {
            return std::stoi(nodeToken.value());
        } else if (nodeToken.type() == token::TokenType::FUNCTION_CALL) {
            std::cout << "uh oh...";
        } else {
            throw std::runtime_error("Unexpected token when evaluating expression");
        }
    } else {
        std::variant<int, float, std::string> left = nodeChildren[0]->eval(env);
        std::variant<int, float, std::string> right = nodeChildren[1]->eval(env);

        if (nodeToken.type() == token::TokenType::OPERATOR_ADD) {
            if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                return std::get<int>(left) + std::get<int>(right);
            } else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
                return std::get<float>(left) + std::get<float>(right);
            } else {
                throw std::runtime_error("Invalid types for addition");
            }
        } else if (nodeToken.type() == token::TokenType::OPERATOR_SUB) {
            if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                return std::get<int>(left) - std::get<int>(right);
            } else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
                return std::get<float>(left) - std::get<float>(right);
            } else {
                throw std::runtime_error("Invalid types for subtraction");
            }
        } else if (nodeToken.type() == token::TokenType::OPERATOR_MUL) {
            if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                return std::get<int>(left) * std::get<int>(right);
            } else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
                return std::get<float>(left) * std::get<float>(right);
            } else {
                throw std::runtime_error("Invalid types for multiplication");
            }
        } else if (nodeToken.type() == token::TokenType::OPERATOR_DIV) {
            if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                return std::get<int>(left) / std::get<int>(right);
            } else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
                return std::get<float>(left) / std::get<float>(right);
            } else {
                throw std::runtime_error("Invalid types for division");
            }
        } else {
            throw std::runtime_error("Unexpected token when evaluating expression");
        }
    }

    return {};
}

ast::ExpressionNode::ExpressionNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(std::move(token), std::move(children)) {
    std::cout << "expression node created";
}

ast::FunctionCallNode::FunctionCallNode(const token::FunctionCallToken& token) : ExpressionNode(token, {}) {
    for (const std::shared_ptr<ast::ExpressionNode>& argument : token.arguments()) {
        nodeChildren.push_back(std::static_pointer_cast<ast::ExpressionNode>(argument));
    }
}

env::VariantType ast::FunctionCallNode::eval(env::Environment& env) const {
    std::cout << "hi";

    std::string functionName = nodeToken.value();

    if (env.getType(functionName) != "function") {
        throw std::runtime_error("Function " + functionName + " is not a function");
    }

    // todo: implement the function call

    return {};  // should return the return value of the function
}
