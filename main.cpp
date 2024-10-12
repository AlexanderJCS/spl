#include <iostream>
#include <memory>
#include <variant>
#include <string>
#include "spl.h"
#include "interpreter/ast.h"

int main() {
    // todo: function calls need a comma at the end of the argument list so the ) token doesn't go into the RPN
    //  parser. This is currently intended behavior but instead should be changed to allow for no comma at the end of
    //  the argument list.
    std::string input = "fun x(a) {b=a;} x(5,);";
    env::Environment env = run(input);

    auto x = std::get<std::shared_ptr<ast::ASTNode>>(env.get("x"));
    std::cout << "x type: " << env.getType("x") << " x children: " << x->children().size() << std::endl;

    return 0;
}