#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "ast.h"


class Parser {
public:
    explicit Parser(std::vector<token::Token> input);

    [[nodiscard]] ast::RootNode root() const;

private:
    /**
     * Parses the input tokens into an AST.
     * @return The root of the AST
     */
    ast::RootNode parse();

    /**
     * Extracts all the tokens within two token types, defined as the start and end tokens. Nested tokens are supported.
     *
     * For example, if the input is:  ((1 + 2) * 3) + 3, and the start token is '(' and the end token is ')', then the
     * function will return the tokens: (1 + 2) * 3
     *
     * Throws an exception if the start token is not the current token.
     *
     * @param start The token type to start extracting from
     * @param end The token type to end extracting at
     * @return The enclosed tokens
     */
    std::vector<token::Token> extractEnclosedTokens(token::TokenType start, token::TokenType end);

    /**
     * Extracts all the tokens within two token types, defined as the start and end tokens. Nested tokens are supported.
     * Different than the other extractEnclosedTokens function, this function returns a vector of shared pointers.
     *
     * For example, if the input is:  ((1 + 2) * 3) + 3, and the start token is '(' and the end token is ')', then the
     * function will return the tokens: (1 + 2) * 3
     *
     * @param start The token type to start extracting from
     * @param end The token type to end extracting at
     * @return The enclosed tokens
     */
    std::vector<std::shared_ptr<token::Token>> extractEnclosedTokensPtr(token::TokenType start, token::TokenType end);

    /**
     * Checks if the parser is at the end of the input (pos >= the size of tokens).
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
     * @return The root of the statement tree. nullptr if the statement cannot be parsed
     */
    std::shared_ptr<ast::ASTNode> parseStatement();

    /**
     * Parses a declaration
     * @return The root of the declaration tree
     */
    std::shared_ptr<ast::DeclarationNode> parseDeclaration();

    /**
     * Parses an expression using the shunting yard algorithm.
     * @return The root of the expression tree
     */
    std::shared_ptr<ast::ExpressionNode> parseExpression();

    /**
     * Parses a function declaration. Assumes the current token is the function name/identifier.
     * @return The root of the function declaration tree
     */
    std::shared_ptr<ast::FunctionDefNode> parseFuncDeclaration();

    /**
     * Parses a jump control flow statement (return, break, continue)
     * @return The root of the control flow tree
     */
    std::shared_ptr<ast::ControlFlowNode> parseControlFlow();

    /**
     * Parses an if statement.
     * @return The root of the if statement tree
     */
    std::shared_ptr<ast::IfNode> parseIf();

    /**
     * Parses a while loop.
     * @return The root of the while loop tree
     */
    std::shared_ptr<ast::WhileNode> parseWhile();

    /**
     * Parses a function call. Assumes the current token is the function name/identifier.
     * @return The function call pseudo-token
     */
    token::FunctionCallToken parseFunctionCall();

    /**
     * Checks if the current token is of the expected type. Throws an exception if it is not. Also advances the parser.
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
