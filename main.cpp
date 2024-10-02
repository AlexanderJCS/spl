#include <iostream>

#include "token/tokenizer.h"
#include "ast/rpn.h"

#include "ast/parser.h"


int main() {
    token::Tokenizer token{"int a = 5;"};

    for (const token::Token& t : token.getTokens()) {
        std::cout << "Token value: " << t.value() << " Token type: " << static_cast<int>(t.type()) << std::endl;
    }

    Parser parser{token.getTokens()};

    std::cout << "Root ast children size: " << parser.root().children().size() << std::endl;

    return 0;
}
