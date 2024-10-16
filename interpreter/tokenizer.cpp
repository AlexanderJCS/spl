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

    // I am referring to 'simple tokens' as tokens that are always a string. For example, equals is always "=". A
    //  complex token, like a LITERAL_INT, can be any characters so long as the follow the rules of an integer.
    std::unordered_map<std::string, TokenType> simpleTokens = {
            {"(", TokenType::OPEN_PAREN},
            {")", TokenType::CLOSE_PAREN},
            {"{", TokenType::OPEN_BRACE},
            {"}", TokenType::CLOSE_BRACE},
            {"=", TokenType::OPERATOR_DEFINE},
            {"+", TokenType::OPERATOR_ADD},
            {"-", TokenType::OPERATOR_SUB},
            {"*", TokenType::OPERATOR_MUL},
            {"/", TokenType::OPERATOR_DIV},
            {",", TokenType::SEPARATOR},
            {";", TokenType::SEMICOLON},
            {"==", TokenType::OPERATOR_EQ},
            {"&&", TokenType::OPERATOR_BOOL_AND},
            {"||", TokenType::OPERATOR_BOOL_OR},
            {"!", TokenType::OPERATOR_UNARY_NOT},
            {"true", TokenType::LITERAL_BOOL},
            {"false", TokenType::LITERAL_BOOL}
    };

    for (char ch : input) {
        // line breaks should increment the line number
        if (ch == '\n') {
            col = 1;
            line++;
        }

        bool isSpace = std::isspace(ch);
        if (isSpace || simpleTokens.count(std::string(1, ch))) {
            processBuffer(buffer, tokens, line, col);

            if (isSpace) {
                continue;
            }
        }

        buffer += ch;

        if (simpleTokens.count(buffer)) {
            tokens.emplace_back(simpleTokens[buffer], buffer, line, col);
            buffer = "";
            continue;
        }

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
