#include "tokenizer.h"

#include <utility>
#include <string>
#include <cctype>


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


token::Tokenizer::Tokenizer(std::string input) {
    tokens = tokenize(std::move(input));
}


void token::Tokenizer::tokenizeIdentifierOrLiteral(std::string &buffer, std::vector<Token> &tokens, size_t line, size_t column){
    if (buffer.empty()) {
        return;
    }

    if (buffer == "int") {
        tokens.emplace_back(TokenType::TYPE_SPECIFIER_INT, buffer, line, column);
    } else if (std::isalpha(buffer[0])) {
        tokens.emplace_back(TokenType::IDENTIFIER, buffer, line, column);
    } else {
        tokens.emplace_back(TokenType::LITERAL_INT, buffer, line, column);
    }

    buffer.clear();
}


std::vector<token::Token> token::Tokenizer::tokenize(std::string input) {
    std::vector<Token> tokens;

    std::string buffer;

    size_t line = 1;
    for (int i = 0; i < input.size(); i++) {
        char ch = input[i];

        // push the token when there's a space
        if (std::isspace(ch)) {
            tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
            continue;  // do not go onto the switch statement
        }

        switch (ch) {
            case '(':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::OPEN_PAREN, "(", line, i);
                break;
            case ')':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::CLOSE_PAREN, ")", line, i);
                break;
            case '{':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::OPEN_BRACE, "{", line, i);
                break;
            case '}':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::CLOSE_BRACE, "}", line, i);
                break;
            case '=':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::OPERATOR_DEFINE, "=", line, i);
                break;
            case ';':
                tokenizeIdentifierOrLiteral(buffer, tokens, line, i);
                tokens.emplace_back(TokenType::SEMICOLON, ";", line, i);
                line++;
                break;
            default:
                buffer += ch;
                break;
        }
    }

    tokenizeIdentifierOrLiteral(buffer, tokens, line, input.size());

    return tokens;
}

std::vector<token::Token> token::Tokenizer::getTokens() const {
    return tokens;
}
