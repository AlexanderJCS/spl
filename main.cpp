#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "a = 0;"
                        "if (false) { a = 1; }"
                        "elif (true) { a = 2; }"
                        "else { a = 3; }";
    env::Environment env = run(input);

    std::cout << std::get<int>(env.get("a")) << std::endl;

    return 0;
}