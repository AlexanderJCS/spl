#include <gtest/gtest.h>

#include <string>

#include "../interpreter/tokenizer.h"

TEST(TokenizerTest, Everything) {
    std::string input = "x ( { } ) 42 4.7 10e3 5. ; = \n \n \t funcCall(a, b) + - * / fun return , && || ! == true false if elif else while break continue < > <= >= % !=";
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
            {token::TokenType::LITERAL_FLOAT, "4.7"},
            {token::TokenType::LITERAL_FLOAT, "10e3"},
            {token::TokenType::LITERAL_FLOAT, "5."},
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
            {token::TokenType::LITERAL_BOOL, "false"},
            {token::TokenType::IF_STATEMENT, ""},
            {token::TokenType::ELIF_STATEMENT, ""},
            {token::TokenType::ELSE_STATEMENT, ""},
            {token::TokenType::WHILE, ""},
            {token::TokenType::BREAK, ""},
            {token::TokenType::CONTINUE, ""},
            {token::TokenType::OPERATOR_LESS, ""},
            {token::TokenType::OPERATOR_GREATER, ""},
            {token::TokenType::OPERATOR_LESS_EQ, ""},
            {token::TokenType::OPERATOR_GREATER_EQ, ""},
            {token::TokenType::OPERATOR_MOD, ""},
            {token::TokenType::OPERATOR_NOT_EQ, ""},
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
