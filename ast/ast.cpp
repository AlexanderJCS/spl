#include "ast.h"

#include <utility>

const token::Token& node::ASTNode::token() const {
    return nodeToken;
}

const std::vector<node::ASTNode>& node::ASTNode::children() const {
    return nodeChildren;
}

int node::ASTNode::line() const {
    return lineAt;
}

int node::ASTNode::column() const {
    return columnAt;
}

node::ASTNode::ASTNode() : nodeType(), lineAt(-1), columnAt(-1) {}

node::ASTNode::ASTNode(NodeType type, token::Token token, std::vector<ASTNode> children)
    : nodeType(type), nodeToken(std::move(token)), nodeChildren(std::move(children)), lineAt(-1), columnAt(-1) {}

std::vector<node::ASTNode> &node::ASTNode::childrenRef() {
    return nodeChildren;
}
