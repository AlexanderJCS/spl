#include "types.h"

#include "control_flow.h"

types::Function::Function(std::vector<std::string> parameters, std::shared_ptr<ast::ASTNode> body)
    : functionParameters(std::move(parameters)), functionBody(std::move(body)) {}

env::VariantType types::Function::call(env::Environment& env, const std::vector<env::VariantType>& args) const {
    for (size_t i = 0; i < functionParameters.size(); i++) {
        env.set(functionParameters[i], args[i]);
    }

    try {
        functionBody->eval(env);
    } catch (const control::ReturnException& e) {
        return e.value();
    }

    return 0;  // todo: create a none type
}
