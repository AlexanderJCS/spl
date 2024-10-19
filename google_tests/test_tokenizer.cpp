#include <gtest/gtest.h>

#include <string>

#include "../interpreter/tokenizer.h"

TEST(TokenizerTest, AlwaysTrue) {
    /*
     * All token types as of writing this test:
     * INVALID,
     * IDENTIFIER,
     * OPEN_PAREN,
     * CLOSE_PAREN,
     * OPEN_BRACE,
     * CLOSE_BRACE,
     * LITERAL_INT,
     * LITERAL_BOOL,
     * SEMICOLON,
     * OPERATOR_DEFINE,
     * OPERATOR_ADD,
     * OPERATOR_SUB,
     * OPERATOR_MUL,
     * OPERATOR_DIV,
     * OPERATOR_BOOL_AND,
     * OPERATOR_BOOL_OR,
     * OPERATOR_UNARY_NOT,
     * OPERATOR_EQ,
     * FUNCTION_DEF,
     * FUNCTION_CALL,
     * RETURN,
     * SEPARATOR
     */

    std::string input = "x ( { } ) 42 ; = \n \n \t funcCall(a, b) + - * / fun return , && || ! == true false";
    token::Tokenizer tokenizer(input);
    std::vector<token::Token> tokens = tokenizer.getTokens();

    // Create expected token types and values
    std::vector<std::pair<token::TokenType, std::string>> expectedTokens = {
            {token::TokenType::IDENTIFIER, "x"},
            {token::TokenType::OPEN_PAREN, ""},
            {token::TokenType::OPEN_BRACE, ""},
            {token::TokenType::CLOSE_BRACE, ""},
            {token::TokenType::CLOSE_PAREN, ""},
            {token::TokenType::LITERAL_INT, "42"},
            {token::TokenType::SEMICOLON, ""},
            {token::TokenType::OPERATOR_DEFINE, ""},
            {token::TokenType::IDENTIFIER, "funcCall"},
            {token::TokenType::OPEN_PAREN, ""},
            {token::TokenType::IDENTIFIER, "a"},
            {token::TokenType::SEPARATOR, ""},
            {token::TokenType::IDENTIFIER, "b"},
            {token::TokenType::CLOSE_PAREN, ""},
            {token::TokenType::OPERATOR_ADD, ""},
            {token::TokenType::OPERATOR_SUB, ""},
            {token::TokenType::OPERATOR_MUL, ""},
            {token::TokenType::OPERATOR_DIV, ""},
            {token::TokenType::FUNCTION_DEF, ""},
            {token::TokenType::RETURN, ""},
            {token::TokenType::SEPARATOR, ""},
            {token::TokenType::OPERATOR_BOOL_AND, ""},
            {token::TokenType::OPERATOR_BOOL_OR, ""},
            {token::TokenType::OPERATOR_UNARY_NOT, ""},
            {token::TokenType::OPERATOR_EQ, ""},
            {token::TokenType::LITERAL_BOOL, "true"},
            {token::TokenType::LITERAL_BOOL, "false"}
    };

    ASSERT_EQ(tokens.size(), expectedTokens.size());

    // Loop through the expected tokens and assert their properties
    for (size_t i = 0; i < expectedTokens.size(); ++i) {
        ASSERT_EQ(tokens[i].type(), expectedTokens[i].first);
        if (!expectedTokens[i].second.empty()) {
            ASSERT_EQ(tokens[i].value(), expectedTokens[i].second);
        }
    }
}
