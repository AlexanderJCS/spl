#ifndef SPL_TYPES_H
#define SPL_TYPES_H

#include <vector>
#include <memory>

#include "ast.h"
#include "environment.h"

namespace types {
    class Function {
    public:
        Function(std::vector<std::string> parameters, std::shared_ptr<ast::ASTNode> body);

        env::VariantType call(env::Environment& env, const std::vector<env::VariantType>& args) const;

    private:
        std::vector<std::string> functionParameters;
        std::shared_ptr<ast::ASTNode> functionBody;
    };
}

#endif  // SPL_TYPES_H
