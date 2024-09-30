
#include "parser.h"

Parser::Parser(std::vector<token::Token> input) {
    tokens = std::move(input);
    pos = 0;

    astRoot = parse();
}

node::ASTNode Parser::parse() {
    node::ASTNode root;


}

const node::ASTNode &Parser::root() const {
    return astRoot;
}

bool Parser::atEnd() {
    return pos >= tokens.size();
}
