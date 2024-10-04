#include <iostream>

#include "token/tokenizer.h"
#include "ast/parser.h"
#include "ast/interpreter.h"

int main() {
    token::Tokenizer token{"mangoes = (3 + 2);"};

    for (const token::Token& t : token.getTokens()) {
        std::cout << "Token value: " << t.value() << " Token type: " << static_cast<int>(t.type()) << std::endl;
    }

    Parser parser{token.getTokens()};

    std::cout << "Root ast children size: " << parser.root().children().size() << std::endl;

    interpreter::Environment env{};

    parser.root().eval(env);

    std::cout << "Value of mangoes: " << std::get<int>(env.get("mangoes")) << std::endl;

    return 0;
}
