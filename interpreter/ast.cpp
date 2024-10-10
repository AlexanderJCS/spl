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

env::VariantType ast::RootNode::eval(env::Environment &env) const {
    for (std::shared_ptr<ast::ASTNode> child : nodeChildren) {
        child->eval(env);
    }

    return {};
}

ast::RootNode::RootNode(const std::vector<std::shared_ptr<ASTNode>> &children) {
    nodeChildren = children;
}


env::VariantType ast::DeclarationNode::eval(env::Environment &env) const {
    token::Token typeSpecifier = nodeToken;
    token::Token identifier = nodeChildren[0]->token();
    env::VariantType value = nodeChildren[1]->eval(env);

    env.set(identifier.value(), value);

    return {};
}

ast::DeclarationNode::DeclarationNode(std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(token::Token(), std::move(children)) {}

env::VariantType ast::StatementNode::eval(env::Environment& env) const {
    return -1;  // not implemented
}

ast::StatementNode::StatementNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(token, std::move(children)) {}

env::VariantType ast::ExpressionNode::eval(env::Environment& env) const {
    if (nodeChildren.empty()) {
        if (nodeToken.type() == token::TokenType::IDENTIFIER) {
            return env.get(nodeToken.value());
        } else if (nodeToken.type() == token::TokenType::LITERAL_INT) {
            return std::stoi(nodeToken.value());
        } else if (nodeToken.type() == token::TokenType::FUNCTION_CALL) {
            // not sure how you got here... but just in case
            throw std::runtime_error("Function calls should be handled by FunctionCallNode");
        } else {
            throw std::runtime_error("Unexpected token when evaluating expression");
        }
    } else {
        env::VariantType left = nodeChildren[0]->eval(env);
        env::VariantType right = nodeChildren[1]->eval(env);

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

ast::ExpressionNode::ExpressionNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(token, std::move(children)) {}

ast::FunctionCallNode::FunctionCallNode(const token::FunctionCallToken& token) : ExpressionNode(token, {}) {
    for (const std::shared_ptr<ast::ExpressionNode>& argument : token.arguments()) {
        nodeChildren.push_back(std::static_pointer_cast<ast::ExpressionNode>(argument));
    }
}

env::VariantType ast::FunctionCallNode::eval(env::Environment& env) const {
    std::string functionName = nodeToken.value();

    if (env.getType(functionName) != "function") {
        throw std::runtime_error("Function " + functionName + " is not a function");
    }

    // todo: implement the function call

    return {};  // should return the return value of the function
}

env::VariantType ast::FunctionDefNode::eval(env::Environment& env) const {
    env.set(nodeToken.value(), functionBody);

    return {};
}

ast::FunctionDefNode::FunctionDefNode(token::Token identifier, std::vector<std::string> args, std::shared_ptr<ASTNode> body) {
    if (identifier.type() != token::TokenType::IDENTIFIER) {
        throw std::runtime_error("FunctionDefNode must be constructed with an identifier token");
    }

    nodeToken = std::move(identifier);
    arguments = std::move(args);
    functionBody = std::move(body);
}
