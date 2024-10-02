#include "ast.h"

#include <utility>

const token::Token& ast::ASTNode::token() const {
    return nodeToken;
}

const std::vector<ast::ASTNode>& ast::ASTNode::children() const {
    return nodeChildren;
}

int ast::ASTNode::line() const {
    return lineAt;
}

int ast::ASTNode::column() const {
    return columnAt;
}

ast::ASTNode::ASTNode() : nodeType(), lineAt(-1), columnAt(-1) {}

ast::ASTNode::ASTNode(NodeType type, token::Token token, std::vector<ASTNode> children)
    : nodeType(type), nodeToken(std::move(token)), nodeChildren(std::move(children)), lineAt(-1), columnAt(-1) {}

std::vector<ast::ASTNode> &ast::ASTNode::childrenRef() {
    return nodeChildren;
}
