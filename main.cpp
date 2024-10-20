#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "fun myFunction(arg1, arg2) {\n"
                        "    return (arg1 + arg2) * 5;\n"
                        "}\n"
                        "\n"
                        "result = myFunction(2.5, 3);\n"
                        "equality = result == 27.5;";
    env::Environment env = run(input);

    std::cout << std::get<bool>(env.get("equality")) << std::endl;

    return 0;
}