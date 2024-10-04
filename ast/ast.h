#ifndef SPL_AST_H
#define SPL_AST_H

#include <variant>
#include <string>
#include <memory>

// forward declaration
namespace interpreter {
    class Environment;
}

#include "interpreter.h"
#include "../token/tokenizer.h"

namespace ast {
    class ASTNode {
    public:
        ASTNode();
        ASTNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children);

        [[nodiscard]] const token::Token& token() const;

        /**
         * @return A reference to the children of this ast.
         */
        [[nodiscard]] std::vector<std::shared_ptr<ASTNode>>& children();

        [[nodiscard]] int line() const;
        [[nodiscard]] int column() const;

        virtual std::variant<int, float, std::string> eval(interpreter::Environment& env) const = 0;

    protected:
        token::Token nodeToken;
        std::vector<std::shared_ptr<ASTNode>> nodeChildren;

        int lineAt;
        int columnAt;
    };

    class RootNode : public ASTNode {
    public:
        RootNode() = default;
        explicit RootNode(const std::vector<std::shared_ptr<ASTNode>>& children);

        std::variant<int, float, std::string> eval(interpreter::Environment& env) const override;
    };

    class DeclarationNode : public ASTNode {
    public:
        DeclarationNode() = default;
        explicit DeclarationNode(std::vector<std::shared_ptr<ASTNode>> children);

        std::variant<int, float, std::string> eval(interpreter::Environment& env) const override;
    };

    class StatementNode : public ASTNode {
    public:
        StatementNode() = default;
        explicit StatementNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children);

        std::variant<int, float, std::string> eval(interpreter::Environment& env) const override;
    };

    class ExpressionNode : public ASTNode {
    public:
        ExpressionNode() = default;
        explicit ExpressionNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children);

        std::variant<int, float, std::string> eval(interpreter::Environment& env) const override;
    };
}


#endif  // SPL_AST_H
