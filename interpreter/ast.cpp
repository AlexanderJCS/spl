#include "ast.h"
#include "control_flow.h"

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

ast::ASTNode::ASTNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children)
    : nodeToken(token), nodeChildren(std::move(children)), lineAt(-1), columnAt(-1) {}

env::VariantType ast::RootNode::eval(env::Environment& env) const {
    for (const std::shared_ptr<ast::ASTNode>& child : nodeChildren) {
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
        switch (nodeToken.type()) {
            case token::TokenType::IDENTIFIER:
                return env.get(nodeToken.value());
            case token::TokenType::LITERAL_INT:
                return std::stoi(nodeToken.value());
            case token::TokenType::LITERAL_BOOL:
                return nodeToken.value() == "true" ? true : false;
            default:
                throw std::runtime_error("Unexpected token when evaluating expression");
        }
    }

    if (nodeChildren.size() == 1) {
        if (nodeToken.type() == token::TokenType::OPERATOR_UNARY_NOT) {
            return !std::get<bool>(nodeChildren[0]->eval(env));
        } else {
            throw std::runtime_error("Unexpected unary operator");
        }
    }

    env::VariantType left = nodeChildren[0]->eval(env);
    env::VariantType right = nodeChildren[1]->eval(env);

    auto applyOperation = [](env::VariantType left, env::VariantType right, auto op) {
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            return env::VariantType(op(std::get<int>(left), std::get<int>(right)));
        } else if (std::holds_alternative<float>(left) && std::holds_alternative<float>(right)) {
            return env::VariantType(op(std::get<float>(left), std::get<float>(right)));
        } else if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
            return env::VariantType(op(std::get<bool>(left), std::get<bool>(right)));
        } else {
            throw std::runtime_error("Invalid types for operation");
        }
    };

    switch (nodeToken.type()) {
        case token::TokenType::OPERATOR_ADD:
            return applyOperation(left, right, std::plus<>{});
        case token::TokenType::OPERATOR_SUB:
            return applyOperation(left, right, std::minus<>{});
        case token::TokenType::OPERATOR_MUL:
            return applyOperation(left, right, std::multiplies<>{});
        case token::TokenType::OPERATOR_DIV:
            return applyOperation(left, right, std::divides<>{});
        case token::TokenType::OPERATOR_EQ:
            return applyOperation(left, right, std::equal_to<>{});
        case token::TokenType::OPERATOR_BOOL_AND:
            return applyOperation(left, right, std::logical_and<>{});
        case token::TokenType::OPERATOR_BOOL_OR:
            return applyOperation(left, right, std::logical_or<>{});
        default:
            throw std::runtime_error("Unexpected token when evaluating expression operator");
    }
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

    types::Function functionBody = std::get<types::Function>(env.get(functionName));

    if (functionBody.parameters().size() != nodeChildren.size()) {
        throw std::runtime_error("Function " + functionName + " expects " + std::to_string(functionBody.parameters().size()) + " arguments, but got " + std::to_string(nodeChildren.size()));
    }

    env::Environment functionScope{env};

    for (size_t i = 0; i < functionBody.parameters().size(); i++) {
        functionScope.set(functionBody.parameters()[i], nodeChildren[i]->eval(env));
    }

    try {
        functionBody.body()->eval(functionScope);
    } catch (const control::ReturnException& e) {
        return e.value();
    }

    return 0;  // todo: implement a null type
}

env::VariantType ast::FunctionDefNode::eval(env::Environment& env) const {
    env.set(nodeToken.value(), types::Function{arguments, functionBody});

    return {};
}

ast::FunctionDefNode::FunctionDefNode(const token::Token& identifier, std::vector<std::string> args, std::shared_ptr<ASTNode> body) {
    if (identifier.type() != token::TokenType::IDENTIFIER) {
        throw std::runtime_error("FunctionDefNode must be constructed with an identifier token");
    }

    nodeToken = identifier;
    arguments = std::move(args);
    functionBody = std::move(body);
}


ast::ControlFlowNode::ControlFlowNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children)
    : ASTNode(token, std::move(children)) {}

env::VariantType ast::ControlFlowNode::eval(env::Environment& env) const {
    switch (nodeToken.type()) {
        case token::TokenType::RETURN:
            throw control::ReturnException(nodeChildren[0]->eval(env));
        default:
            throw std::runtime_error("Unexpected control flow token");
    }
}
