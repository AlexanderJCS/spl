#include <gtest/gtest.h>

#include <string>

#include "../interpreter/tokenizer.h"

TEST(TokenizerTest, AlwaysTrue) {
    // INVALID,
    // IDENTIFIER,
    // OPEN_PAREN,
    // CLOSE_PAREN,
    // OPEN_BRACE,
    // CLOSE_BRACE,
    // LITERAL_INT,
    // SEMICOLON,
    // OPERATOR_DEFINE,
    // OPERATOR_ADD,
    // OPERATOR_SUB,
    // OPERATOR_MUL,
    // OPERATOR_DIV

    std::string input = "x ( { } ) 42 ; = \n \n   \t  + - * /";

    token::Tokenizer tokenizer = token::Tokenizer(input);
    std::vector<token::Token> tokens = tokenizer.getTokens();

    ASSERT_EQ(tokens.size(), 12);

    ASSERT_EQ(tokens[0].type(), token::TokenType::IDENTIFIER);
    ASSERT_EQ(tokens[0].value(), "x");

    ASSERT_EQ(tokens[1].type(), token::TokenType::OPEN_PAREN);

    ASSERT_EQ(tokens[2].type(), token::TokenType::OPEN_BRACE);

    ASSERT_EQ(tokens[3].type(), token::TokenType::CLOSE_BRACE);

    ASSERT_EQ(tokens[4].type(), token::TokenType::CLOSE_PAREN);

    ASSERT_EQ(tokens[5].type(), token::TokenType::LITERAL_INT);
    ASSERT_EQ(tokens[5].value(), "42");

    ASSERT_EQ(tokens[6].type(), token::TokenType::SEMICOLON);

    ASSERT_EQ(tokens[7].type(), token::TokenType::OPERATOR_DEFINE);

    ASSERT_EQ(tokens[8].type(), token::TokenType::OPERATOR_ADD);

    ASSERT_EQ(tokens[9].type(), token::TokenType::OPERATOR_SUB);

    ASSERT_EQ(tokens[10].type(), token::TokenType::OPERATOR_MUL);

    ASSERT_EQ(tokens[11].type(), token::TokenType::OPERATOR_DIV);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
