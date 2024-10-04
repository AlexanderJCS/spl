#include "interpreter.h"

#include <stdexcept>
#include <utility>

interpreter::Environment::Environment() : parent(nullptr) {}

interpreter::Environment::Environment(const Environment& parent) : parent(&parent) {}

void interpreter::Environment::set(const std::string& name, VariantType value) {
    variables[name] = std::move(value);
}

bool interpreter::Environment::has(const std::string& name) const {
    return variables.find(name) != variables.end();
}

interpreter::VariantType interpreter::Environment::get(const std::string& name) const {
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

void interpreter::Environment::remove(const std::string &name) {
    variables.erase(name);
}

std::string interpreter::Environment::getType(const std::string& name) const {
    return std::visit([](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) {
            return "int";
        } else if constexpr (std::is_same_v<T, float>) {
            return "float";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "string";
        } else {
            throw std::runtime_error("Unknown type");
        }
    }, get(name));
}
