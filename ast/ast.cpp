#include "ast.h"

#include <stdexcept>
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

ast::NodeType ast::ASTNode::type() const {
    return nodeType;
}

std::variant<int, float, std::string> ast::ASTNode::eval(interpreter::Environment &env) const {
    switch (nodeType) {
        case NodeType::ROOT: {
            for (const ASTNode& child : nodeChildren) {
                child.eval(env);
            }

            break;
        }

        case NodeType::DECLARATION: {
            token::Token typeSpecifier = nodeToken;
            token::Token identifier = nodeChildren[0].token();
            std::variant<int, float, std::string> value = nodeChildren[1].eval(env);

            env.set(identifier.value(), value);

            break;
        }

        case NodeType::STATEMENT:
            return -1;  // not implemented

        case NodeType::EXPRESSION:
            if (nodeChildren.empty()) {
                if (nodeToken.type() == token::TokenType::IDENTIFIER) {
                    return env.get(nodeToken.value());
                } else if (nodeToken.type() == token::TokenType::LITERAL_INT) {
                    return std::stoi(nodeToken.value());
                } else {
                    throw std::runtime_error("Unexpected token");
                }
            } else {
                std::variant<int, float, std::string> left = nodeChildren[0].eval(env);
                std::variant<int, float, std::string> right = nodeChildren[1].eval(env);

                // todo: iterate through all operators and apply the correct one
            }

            break;
        }

    return {};  // uh oh
}
