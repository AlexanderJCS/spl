#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    Parser(std::vector<token::Token> input);

    [[nodiscard]] ast::RootNode root() const;

private:
    /**
     * Parses the input tokens into an AST.
     * @return The root of the AST
     */
    ast::RootNode parse();

    /**
     * Goes through each token and, if it is a function declaration, changes the token type to FUNCTION_CALL.
     * Mutates the tokens vector. Intended to be called before parse().
     */
    void addFunctionCalls();

    /**
     * Checks if the parser is at the end of the input.
     * @return True if the parser is at the end of the input, false otherwise
     */
    [[nodiscard]] bool atEnd() const;

    /**
     * Returns the current token.
     * @return The current token
     */
    [[nodiscard]] token::Token currentToken() const;

    /**
     * Peeks at the next token.
     * @return The next token
     */
    [[nodiscard]] token::Token peek() const;

    /**
     * Advances the parser by one token.
     * @return The token before advancing
     */
    token::Token advance();

    /**
     * Parses a generalized statement.
     * @return The root of the statement tree
     */
    std::shared_ptr<ast::ASTNode> parseStatement();

    /**
     * Parses a declaration
     * @return The root of the declaration tree
     */
    ast::DeclarationNode parseDeclaration();

    /**
     * Parses an expression using the shunting yard algorithm.
     * @return The root of the expression tree
     */
    ast::ExpressionNode parseExpression();

    /**
     * Checks if the current token is of the expected type. Throws an exception if it is not.
     *
     * @throws std::runtime_error if the current token is not of the expected type
     * @param type The expected type of the token
     */
    void expect(token::TokenType type);

    ast::RootNode astRoot;
    std::vector<token::Token> tokens;
    int pos;
};

#endif  // SPL_PARSER_H