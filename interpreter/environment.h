#ifndef SPL_ENVIRONMENT_H
#define SPL_ENVIRONMENT_H

#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <memory>

// Forward declarations
namespace ast {
    class ASTNode;
}

namespace types {
    class Function {
    public:
        Function(std::vector<std::string> parameters, std::shared_ptr<ast::ASTNode> body);

        [[nodiscard]] const std::vector<std::string>& parameters() const;
        [[nodiscard]] const std::shared_ptr<ast::ASTNode>& body() const;

        bool operator==(const Function& other) const;

    private:
        std::vector<std::string> functionParameters;
        std::shared_ptr<ast::ASTNode> functionBody;
    };
}

namespace env {
    using VariantType = std::variant<bool, int, float, std::string, types::Function>;

    class Environment {
    public:
        Environment();
        Environment(Environment& parent);

        /**
         * Sets a variable in the environment
         * @param name The name of the variable
         * @param value The value of the variable
         */
        void set(const std::string& name, VariantType value);

        /**
         * Gets a variable from the environment. Throws an exception if the variable is not in the environment
         * @param name The name of the variable
         * @return The value of the variable
         */
        VariantType get(const std::string& name) const;

        /**
         * Gets the type of a variable in the environment as a string. Possible types:
         * - "int"
         * - "float"
         * - "string"
         * - "ast" (an ast::ASTNode shared_ptr. Used for function definitions)
         *
         * @throws std::runtime_error if the variable is not in the environment
         * @param name The name of the variable
         * @return The type of the variable as a string
         */
        std::string getType(const std::string& name) const;

        /**
         * Checks if a variable is in the environment or any parent environments
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
        std::unordered_map<std::string, VariantType> variables;
        Environment* parent;  // may be nullptr
    };
}

#endif  // SPL_ENVIRONMENT_H
