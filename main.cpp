#include <iostream>
#include <memory>
#include <variant>
#include <string>
#include "spl.h"
#include "interpreter/ast.h"

int main() {
    std::string input = "fun x() {b = 3;} x();";
    env::Environment env = run(input);

    auto x = std::get<std::shared_ptr<ast::ASTNode>>(env.get("x"));
    std::cout << "x type: " << env.getType("x") << " x children: " << x->children().size() << std::endl;

    return 0;
}