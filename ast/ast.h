#ifndef SPL_AST_H
#define SPL_AST_H

#include "../token/tokenizer.h"

namespace node {
    enum class NodeType {
        DECLARATION,
        STATEMENT,
        EXPRESSION
    };

    class ASTNode {
    public:
        ASTNode();
        ASTNode(NodeType type, token::Token token, std::vector<ASTNode> children);

        [[nodiscard]] const token::Token& token() const;
        [[nodiscard]] const std::vector<ASTNode>& children() const;

        [[nodiscard]] int line() const;
        [[nodiscard]] int column() const;

    private:
        NodeType nodeType;
        token::Token nodeToken;
        std::vector<ASTNode> nodeChildren;

        int lineAt;
        int columnAt;
    };
}


#endif  // SPL_AST_H
