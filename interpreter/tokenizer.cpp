#include "tokenizer.h"

#include <stdexcept>
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


void token::Tokenizer::processComplexToken(
        std::string& buffer, std::vector<Token>& tokens, size_t line, size_t column
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
        bool isInteger = true;
        for (char ch : buffer) {
            if (ch == '.' || ch == 'e' || ch == 'E') {
                isInteger = false;
                break;
            } else if (!std::isdigit(ch)) {
                // not a number
                throw std::runtime_error("Unexpected character in number: " + buffer);
            }
        }


        tokens.emplace_back(isInteger ? TokenType::LITERAL_INT : TokenType::LITERAL_FLOAT, buffer, line, column);
    }

    buffer.clear();
}



std::vector<token::Token> token::Tokenizer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string buffer;
    size_t line = 1;
    size_t col = 0;  // set to 0 because the loops increments immediately

    for (int i = 0; i < input.size(); i++) {
        char ch = input[i];
        col++;

        // line breaks should increment the line number
        if (ch == '\n') {
            col = 0;
            line++;
        }

        /*
         * I have no idea why the below code works, but it does. If anyone figures it out, please let me know so I can
         * notify the Nobel Prize committee and recommend that you are awarded it immediately.
         */

        // 1st condition: if the character is a space
        // 2nd condition: if the character is moving from a complex token/identifier to a simple token
        bool isSpace = std::isspace(ch);
        if (isSpace || (!buffer.empty() && !simpleTokens.count(buffer) && simpleTokens.count(std::string(1, ch)))) {
            processComplexToken(buffer, tokens, line, col);

            if (isSpace) {
                continue;
            }
        }

        buffer += ch;

        // handles the case where "=" is a token but "==" also is a token
        bool nextBufferIsToken = i + 1 < input.size() && simpleTokens.count(buffer + input[i + 1]);

        if (simpleTokens.count(buffer) && !nextBufferIsToken) {
            tokens.emplace_back(simpleTokens.at(buffer), buffer, line, col);
            buffer = "";
            continue;
        }
    }

    // finalize the last token
    processComplexToken(buffer, tokens, line, input.size());

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
