#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "a = 0; if (true) { a = 1; }";
    env::Environment env = run(input);

    std::cout << std::get<int>(env.get("a")) << std::endl;

    return 0;
}