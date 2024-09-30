#ifndef SPL_TOKENIZER_H
#define SPL_TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace token {
    enum class TokenType {
        IDENTIFIER,
        OPEN_PAREN,
        CLOSE_PAREN,
        OPEN_BRACE,
        CLOSE_BRACE,
        LITERAL_INT,
        TYPE_SPECIFIER_INT,
        SEMICOLON,
        OPERATOR_EQUAL
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
            {TokenType::OPERATOR_EQUAL, 1},
    };

    const std::unordered_map<TokenType, Associativity> operatorAssociativityMap = {
            {TokenType::OPERATOR_EQUAL, Associativity::RIGHT}
    };

    class Token {
    public:
        Token(TokenType type, std::string value, size_t line, size_t column);

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

    class Tokenizer {
    public:
        explicit Tokenizer(std::string input);

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
        static void tokenizeIdentifierOrLiteral(std::string& buffer, std::vector<Token>& tokens, size_t line, size_t column);
        static std::vector<Token> tokenize(std::string input);
    };
}

#endif  // SPL_TOKENIZER_H
