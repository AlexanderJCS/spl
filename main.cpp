#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "b = 3 == 3;\n";
    env::Environment env = run(input);

    std::cout << std::get<int>(env.get("b")) << std::endl;

    return 0;
}