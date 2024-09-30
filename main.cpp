#include <iostream>

#include "token/tokenizer.h"
#include "ast/rpn.h"

int main() {
    token::Tokenizer token{"int a = 5;"};

    for (const token::Token& t : token.getTokens()) {
        std::cout << "Token value: " << t.value() << " Token type: " << static_cast<int>(t.type()) << std::endl;
    }

    ShuntingYardParser parser{token.getTokens()};

    std::cout << "finished" << std::endl;

    return 0;
}
