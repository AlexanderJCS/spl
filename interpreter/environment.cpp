#include "environment.h"

#include <stdexcept>
#include <utility>

env::Environment::Environment() : parent(nullptr) {}

env::Environment::Environment(Environment& parent) : parent(&parent) {}

void env::Environment::set(const std::string& name, VariantType value) {
    if (has(name) && !variables.count(name)) {
        parent->set(name, std::move(value));
        return;
    }

    variables[name] = std::move(value);
}

bool env::Environment::has(const std::string& name) const {
    const Environment* current = this;

    while (current != nullptr) {
        if (current->variables.count(name)) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

env::VariantType env::Environment::get(const std::string& name) const {
    // use an iterative approach so clang-tidy doesn't complain about recursion
    const Environment* current = this;

    while (current != nullptr) {
        auto it = current->variables.find(name);
        if (it != current->variables.end()) {
            return it->second;
        }

        current = current->parent;
    }

    throw std::runtime_error("Variable not found: " + name);
}

void env::Environment::remove(const std::string &name) {
    variables.erase(name);
}

std::string env::Environment::getType(const std::string& name) const {
    return std::visit([](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            return "int";
        } else if constexpr (std::is_same_v<T, float>) {
            return "float";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "string";
        } else if constexpr (std::is_same<T, types::Function>::value) {
            return "function";
        } else {
            throw std::runtime_error("Unknown type");
        }
    }, get(name));
}

types::Function::Function(std::vector<std::string> parameters, std::shared_ptr<ast::ASTNode> body)
    : functionParameters(std::move(parameters)), functionBody(std::move(body)) {}

const std::vector<std::string>& types::Function::parameters() const {
    return functionParameters;
}

const std::shared_ptr<ast::ASTNode>& types::Function::body() const {
    return functionBody;
}

bool types::Function::operator==(const types::Function& other) const {
    return functionParameters == other.functionParameters && functionBody == other.functionBody;
}
