#include "ast.h"

#include <stdexcept>
#include <utility>
#include <memory>

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

std::variant<int, float, std::string> ast::RootNode::eval(interpreter::Environment &env) const {
    for (std::shared_ptr<ast::ASTNode> child : nodeChildren) {
        child->eval(env);
    }

    return {};
}

ast::RootNode::RootNode(const std::vector<std::shared_ptr<ASTNode>> &children) {
    nodeChildren = children;
}


std::variant<int, float, std::string> ast::DeclarationNode::eval(interpreter::Environment &env) const {
    token::Token typeSpecifier = nodeToken;
    token::Token identifier = nodeChildren[0]->token();
    std::variant<int, float, std::string> value = nodeChildren[1]->eval(env);

    env.set(identifier.value(), value);

    return {};
}

ast::DeclarationNode::DeclarationNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children) : ASTNode(std::move(token), children) {}

std::variant<int, float, std::string> ast::StatementNode::eval(interpreter::Environment &env) const {
    return -1;  // not implemented
}

ast::StatementNode::StatementNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(std::move(token), std::move(children)) {}

std::variant<int, float, std::string> ast::ExpressionNode::eval(interpreter::Environment &env) const {
    if (nodeChildren.empty()) {
        if (nodeToken.type() == token::TokenType::IDENTIFIER) {
            return env.get(nodeToken.value());
        } else if (nodeToken.type() == token::TokenType::LITERAL_INT) {
            return std::stoi(nodeToken.value());
        } else {
            throw std::runtime_error("Unexpected token");
        }
    } else {
        std::variant<int, float, std::string> left = nodeChildren[0]->eval(env);
        std::variant<int, float, std::string> right = nodeChildren[1]->eval(env);

        // todo: iterate through all operators and apply the correct one
    }
}

ast::ExpressionNode::ExpressionNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children) : ASTNode(token, children) {

}
