#include "tokenizer.h"

#include <utility>
#include <string>
#include <cctype>

#include "ast.h"


token::Token::Token(TokenType type, std::string value, size_t line, size_t column)
    : tokenType(type), tokenValue(std::move(value)), lineAt(line), columnAt(column) {}

token::TokenType token::Token::type() const {
    return tokenType;
}

size_t token::Token::line() const {
    return lineAt;
}

std::string token::Token::value() const {
    return tokenValue;
}

size_t token::Token::column() const {
    return columnAt;
}


/**
 *
 * @return
 */
int token::Token::precedence() const {
    if (operatorPrecedenceMap.find(tokenType) == operatorPrecedenceMap.end()) {
        return -1;
    }

    return operatorPrecedenceMap.at(tokenType);;
}

token::Associativity token::Token::associativity() const {
    if (operatorAssociativityMap.find(tokenType) == operatorAssociativityMap.end()) {
        return Associativity::NOT_APPLICABLE;
    }

    return operatorAssociativityMap.at(tokenType);
}

token::Token::Token() {
    tokenType = TokenType::INVALID;
    tokenValue = "";
    lineAt = 0;
    columnAt = 0;
}


token::Tokenizer::Tokenizer(const std::string& input) {
    tokens = tokenize(input);
}


void token::Tokenizer::processBuffer(
        std::string &buffer, std::vector<Token>& tokens, size_t line, size_t column
) {
    if (buffer.empty()) {
        return;
    }

    if (buffer == "fun") {
        tokens.emplace_back(TokenType::FUNCTION_DEF, buffer, line, column);
    } else if (buffer == "return") {
        tokens.emplace_back(TokenType::RETURN, buffer, line, column);
    } else if (std::isalpha(buffer[0])) {
        tokens.emplace_back(TokenType::IDENTIFIER, buffer, line, column);
    } else {
        tokens.emplace_back(TokenType::LITERAL_INT, buffer, line, column);
    }

    buffer.clear();
}



std::vector<token::Token> token::Tokenizer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string buffer;
    size_t line = 1;
    size_t col = 1;

    std::unordered_map<char, TokenType> singleCharTokens = {
            {'(', TokenType::OPEN_PAREN},
            {')', TokenType::CLOSE_PAREN},
            {'{', TokenType::OPEN_BRACE},
            {'}', TokenType::CLOSE_BRACE},
            {'=', TokenType::OPERATOR_DEFINE},
            {'+', TokenType::OPERATOR_ADD},
            {'-', TokenType::OPERATOR_SUB},
            {'*', TokenType::OPERATOR_MUL},
            {'/', TokenType::OPERATOR_DIV},
            {',', TokenType::SEPARATOR},
            {';', TokenType::SEMICOLON},
    };

    for (char ch : input) {
        // line breaks should increment the line number
        if (ch == '\n') {
            col = 1;
            line++;
        }

        // finalize the current token if the character is whitespace
        if (std::isspace(ch)) {
            processBuffer(buffer, tokens, line, col);
            continue;
        }

        // handle single character tokens
        if (singleCharTokens.count(ch)) {
            processBuffer(buffer, tokens, line, col);  // Finalize any buffer before pushing the token
            tokens.emplace_back(singleCharTokens[ch], std::string(1, ch), line, col);
            continue;
        }

        // default case, add to buffer (for identifiers/literals/keywords)
        buffer += ch;
        col++;
    }

    // finalize the last token
    processBuffer(buffer, tokens, line, input.size());

    return tokens;
}

std::vector<token::Token> token::Tokenizer::getTokens() const {
    return tokens;
}

token::FunctionCallToken::FunctionCallToken(const std::string& functionName, size_t line, size_t column,
                                            std::vector<std::shared_ptr<ast::ExpressionNode>> arguments)
        : Token(TokenType::FUNCTION_CALL, functionName, line, column), functionArguments(std::move(arguments)) {
}

std::vector<std::shared_ptr<ast::ExpressionNode>> token::FunctionCallToken::arguments() const {
    return functionArguments;
}
