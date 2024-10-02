#ifndef SPL_AST_H
#define SPL_AST_H

#include <variant>
#include <string>

// forward declaration
namespace interpreter {
    class Environment;
}

#include "interpreter.h"
#include "../token/tokenizer.h"

namespace ast {
    enum class NodeType {
        ROOT,
        DECLARATION,
        STATEMENT,
        EXPRESSION
    };

    class ASTNode {
    public:
        ASTNode();
        ASTNode(NodeType type, token::Token token, std::vector<ASTNode> children);

        [[nodiscard]] NodeType type() const;
        [[nodiscard]] const token::Token& token() const;
        [[nodiscard]] const std::vector<ASTNode>& children() const;

        /**
         * TODO: this is a hack. I feel like this class should be immutable but this saves time for the moment.
         * @return The children of this ast.
         */
        [[nodiscard]] std::vector<ASTNode>& childrenRef();

        [[nodiscard]] int line() const;
        [[nodiscard]] int column() const;

        std::variant<int, float, std::string> eval(interpreter::Environment& env) const;

    private:
        NodeType nodeType;
        token::Token nodeToken;
        std::vector<ASTNode> nodeChildren;

        int lineAt;
        int columnAt;
    };
}


#endif  // SPL_AST_H
