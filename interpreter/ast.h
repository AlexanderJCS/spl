#ifndef SPL_AST_H
#define SPL_AST_H

#include <variant>
#include <string>
#include <memory>
#include <vector>

// forward declaration
namespace env {
    using VariantType = std::variant<int, float, std::string>;

    class Environment;
}

namespace token {
    class Token;
}

#include "environment.h"
#include "tokenizer.h"


namespace ast {
    class ASTNode {
    public:
        ASTNode();
        ASTNode(token::Token token, std::vector<std::shared_ptr<ASTNode>> children);

        [[nodiscard]] const token::Token& token() const;

        /**
         * @return A reference to the children of this interpreter.
         */
        [[nodiscard]] std::vector<std::shared_ptr<ASTNode>>& children();

        [[nodiscard]] int line() const;
        [[nodiscard]] int column() const;

        virtual env::VariantType eval(env::Environment& env) const = 0;

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

        env::VariantType eval(env::Environment& env) const override;
    };

    class DeclarationNode : public ASTNode {
    public:
        DeclarationNode() = default;
        explicit DeclarationNode(std::vector<std::shared_ptr<ASTNode>> children);

        env::VariantType eval(env::Environment& env) const override;
    };

    class StatementNode : public ASTNode {
    public:
        StatementNode() = default;
        explicit StatementNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children);

        env::VariantType eval(env::Environment& env) const override;
    };

    class ExpressionNode : public ASTNode {
    public:
        ExpressionNode() = default;
        explicit ExpressionNode(const token::Token& token, std::vector<std::shared_ptr<ASTNode>> children);

        env::VariantType eval(env::Environment& env) const override;
    };

    class FunctionCallNode : public ExpressionNode {
    public:
        FunctionCallNode() = default;

        /**
         * Construct a FunctionCallNode with a token and children.
         * @param token The token of the node. Should be an identifier and contain the name of the function.
         * @param children The arguments to the function. Each child should be an ExpressionNode.
         */
        explicit FunctionCallNode(const token::FunctionCallToken& token);

        env::VariantType eval(env::Environment& env) const override;
    };
}


#endif  // SPL_AST_H
