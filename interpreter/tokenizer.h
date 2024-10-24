#ifndef SPL_TOKENIZER_H
#define SPL_TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace ast {
    class ExpressionNode;  // Forward declaration of ast::ExpressionNode
    class FunctionCallNode;  // Forward declaration of ast::FunctionCallNode
}

namespace token {
    enum class TokenType {
        INVALID,
        IDENTIFIER,
        OPEN_PAREN,
        CLOSE_PAREN,
        OPEN_BRACE,
        CLOSE_BRACE,
        LITERAL_INT,
        LITERAL_BOOL,
        LITERAL_FLOAT,
        SEMICOLON,
        OPERATOR_DEFINE,
        OPERATOR_ADD,
        OPERATOR_SUB,
        OPERATOR_MUL,
        OPERATOR_DIV,
        OPERATOR_MOD,
        OPERATOR_NOT_EQ,
        OPERATOR_LESS,
        OPERATOR_LESS_EQ,
        OPERATOR_GREATER,
        OPERATOR_GREATER_EQ,
        OPERATOR_BOOL_AND,
        OPERATOR_BOOL_OR,
        OPERATOR_UNARY_NOT,
        OPERATOR_EQ,
        FUNCTION_DEF,
        FUNCTION_CALL,
        RETURN,
        SEPARATOR,
        IF_STATEMENT,
        ELIF_STATEMENT,
        ELSE_STATEMENT,
        WHILE,
        CONTINUE,
        BREAK
    };

    /**
     * The associativity of an operator. NOT_APPLICABLE means that the token is not an operator.
     */
    enum class Associativity {
        LEFT,
        RIGHT,
        NOT_APPLICABLE
    };

    const std::unordered_map<TokenType, int> operatorPrecedenceMap = {
            {TokenType::OPERATOR_DEFINE,     1},   // Assignment has lowest precedence
            {TokenType::OPERATOR_BOOL_OR,    2},   // Logical OR
            {TokenType::OPERATOR_BOOL_AND,   3},   // Logical AND
            {TokenType::OPERATOR_EQ,         4},   // Equality checks
            {TokenType::OPERATOR_NOT_EQ,     4},
            {TokenType::OPERATOR_LESS,       5},   // Relational (>, <, >=, <=)
            {TokenType::OPERATOR_LESS_EQ,    5},
            {TokenType::OPERATOR_GREATER,    5},
            {TokenType::OPERATOR_GREATER_EQ, 5},
            {TokenType::OPERATOR_ADD,        6},   // Addition/Subtraction
            {TokenType::OPERATOR_SUB,        6},
            {TokenType::OPERATOR_MUL,        7},   // Multiplication/Division/Modulus
            {TokenType::OPERATOR_DIV,        7},
            {TokenType::OPERATOR_MOD,        7},
            {TokenType::OPERATOR_UNARY_NOT,  8}    // Unary NOT (higher precedence than relational and arithmetic)
    };

    const std::unordered_map<TokenType, Associativity> operatorAssociativityMap = {
            {TokenType::OPERATOR_DEFINE,     Associativity::RIGHT},
            {TokenType::OPERATOR_ADD,        Associativity::LEFT},
            {TokenType::OPERATOR_SUB,        Associativity::LEFT},
            {TokenType::OPERATOR_MUL,        Associativity::LEFT},
            {TokenType::OPERATOR_DIV,        Associativity::LEFT},
            {TokenType::OPERATOR_BOOL_AND,   Associativity::LEFT},
            {TokenType::OPERATOR_BOOL_OR,    Associativity::LEFT},
            {TokenType::OPERATOR_UNARY_NOT,  Associativity::RIGHT},
            {TokenType::OPERATOR_EQ,         Associativity::LEFT},
            {TokenType::OPERATOR_NOT_EQ,     Associativity::LEFT},
            {TokenType::OPERATOR_LESS,       Associativity::LEFT},
            {TokenType::OPERATOR_LESS_EQ,    Associativity::LEFT},
            {TokenType::OPERATOR_GREATER,    Associativity::LEFT},
            {TokenType::OPERATOR_GREATER_EQ, Associativity::LEFT},
            {TokenType::OPERATOR_MOD,        Associativity::LEFT}
    };

    const std::unordered_map<std::string, TokenType> simpleTokens = {
            {"(", TokenType::OPEN_PAREN},
            {")", TokenType::CLOSE_PAREN},
            {"{", TokenType::OPEN_BRACE},
            {"}", TokenType::CLOSE_BRACE},
            {"=", TokenType::OPERATOR_DEFINE},
            {"+", TokenType::OPERATOR_ADD},
            {"-", TokenType::OPERATOR_SUB},
            {"*", TokenType::OPERATOR_MUL},
            {"/", TokenType::OPERATOR_DIV},
            {",", TokenType::SEPARATOR},
            {";", TokenType::SEMICOLON},
            {"==", TokenType::OPERATOR_EQ},
            {"!=", TokenType::OPERATOR_NOT_EQ},
            {"&&", TokenType::OPERATOR_BOOL_AND},
            {"||", TokenType::OPERATOR_BOOL_OR},
            {"!", TokenType::OPERATOR_UNARY_NOT},
            {"true", TokenType::LITERAL_BOOL},
            {"false", TokenType::LITERAL_BOOL},
            {"if", TokenType::IF_STATEMENT},
            {"elif", TokenType::ELIF_STATEMENT},
            {"else", TokenType::ELSE_STATEMENT},
            {"while", TokenType::WHILE},
            {"continue", TokenType::CONTINUE},
            {"break", TokenType::BREAK},
            {"%", TokenType::OPERATOR_MOD},
            {"<", TokenType::OPERATOR_LESS},
            {">", TokenType::OPERATOR_GREATER},
            {"<=", TokenType::OPERATOR_LESS_EQ},
            {">=", TokenType::OPERATOR_GREATER_EQ}
    };

    class Token {
    public:
        Token();
        Token(TokenType type, std::string value, size_t line, size_t column);
        virtual ~Token() = default;  // virtual destructor to allow for polymorphism... I hate c++

        [[nodiscard]] TokenType type() const;
        [[nodiscard]] std::string value() const;
        [[nodiscard]] size_t line() const;
        [[nodiscard]] size_t column() const;

        /**
         * Get the precedence of the token. If the token is not an operator, then it will return -1.
         * @return The precedence of the token.
         */
        [[nodiscard]] int precedence() const;

        /**
         * Get the associativity of the token. If the token is not an operator, then it will return
         * Associativity::NOT_APPLICABLE.
         * @return The associativity of the token.
         */
        [[nodiscard]] Associativity associativity() const;

    private:
        TokenType tokenType;
        std::string tokenValue;
        size_t lineAt;
        size_t columnAt;
    };

    /**
     * A special "pseudo-token" for function calls. This token is used to represent a function call so it can be
     * passed to the Shunting Yard Parser.
     */
    class FunctionCallToken : public Token {
    public:
        FunctionCallToken(const std::string &functionName, size_t line, size_t column,
                          std::vector<std::shared_ptr<ast::ExpressionNode>> arguments);

        ~FunctionCallToken() override = default;

        [[nodiscard]] std::vector<std::shared_ptr<ast::ExpressionNode>> arguments() const;

    private:
        std::vector<std::shared_ptr<ast::ExpressionNode>> functionArguments;
    };

    class Tokenizer {
    public:
        explicit Tokenizer(const std::string& input);

        [[nodiscard]] std::vector<Token> getTokens() const;

    private:
        std::vector<Token> tokens;

        /**
         * Helper function for tokenizing identifiers and literals. Since this code is repeated a lot in the tokenize function,
         * it's best to make a function.
         *
         * @param buffer The buffer of the current token. If empty, then the function will not do anything.
         * @param tokens The vector of tokens to add the token to.
         * @param line The line number of the token.
         * @param column The column number of the token.
         */
        static void processComplexToken(std::string& buffer, std::vector<Token>& tokens, size_t line, size_t column);
        static std::vector<Token> tokenize(const std::string& input);
    };
}

#endif  // SPL_TOKENIZER_H
