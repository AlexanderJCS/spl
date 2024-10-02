#ifndef SPL_INTERPRETER_H
#define SPL_INTERPRETER_H

#include <variant>
#include <string>
#include <optional>
#include <unordered_map>

#include "ast.h"

namespace interpreter {
    class Environment {
    public:
        Environment();
        Environment(const Environment& parent);

        /**
         * Sets a variable in the environment
         * @param name The name of the variable
         * @param value The value of the variable
         */
        void set(const std::string& name, std::variant<int, float, std::string> value);

        /**
         * Gets a variable from the environment. Throws an exception if the variable is not in the environment
         * @param name The name of the variable
         * @return The value of the variable
         */
        std::variant<int, float, std::string> get(const std::string& name) const;

        /**
         * Checks if a variable is in the environment
         * @param name The name of the variable
         * @return True if the variable is in the environment, false otherwise
         */
        bool has(const std::string& name) const;

        /**
         * Removes a variable from the environment
         * @param name The name of the variable to remove
         */
        void remove(const std::string& name);

    private:
        std::unordered_map<std::string, std::variant<int, float, std::string>> variables;
        const Environment* parent;  // may be nullptr
    };
}

#endif  // SPL_INTERPRETER_H
