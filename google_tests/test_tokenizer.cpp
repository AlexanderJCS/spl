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
     * SEMICOLON,
     * OPERATOR_DEFINE,
     * OPERATOR_ADD,
     * OPERATOR_SUB,
     * OPERATOR_MUL,
     * OPERATOR_DIV
     */

    std::string input = "x ( { } ) 42 ; = \n \n   \t  + - * /";
    token::Tokenizer tokenizer(input);
    std::vector<token::Token> tokens = tokenizer.getTokens();

    ASSERT_EQ(tokens.size(), 12);

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
            {token::TokenType::OPERATOR_ADD, ""},
            {token::TokenType::OPERATOR_SUB, ""},
            {token::TokenType::OPERATOR_MUL, ""},
            {token::TokenType::OPERATOR_DIV, ""}
    };

    // Loop through the expected tokens and assert their properties
    for (size_t i = 0; i < expectedTokens.size(); ++i) {
        ASSERT_EQ(tokens[i].type(), expectedTokens[i].first);
        if (!expectedTokens[i].second.empty()) {
            ASSERT_EQ(tokens[i].value(), expectedTokens[i].second);
        }
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
