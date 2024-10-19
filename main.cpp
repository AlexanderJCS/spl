#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "b = (!(3 == 4) && true) || false;\n";
    env::Environment env = run(input);

    std::cout << std::get<bool>(env.get("b")) << std::endl;

    return 0;
}