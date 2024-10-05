#include <string>

#include "interpreter/tokenizer.h"
#include "interpreter/environment.h"
#include "interpreter/parser.h"


env::Environment run(const std::string& input) {
    token::Tokenizer token{input};

    Parser parser{token.getTokens()};
    env::Environment env;

    parser.root().eval(env);

    return env;
}
