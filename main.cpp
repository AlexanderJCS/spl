#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "fun sub(a, b) {\n"
                        "    return a - b;\n"
                        "}\n"
                        "\n"
                        "fun add(a, b) {\n"
                        "    return a + b;\n"
                        "}\n"
                        "\n"
                        "b = 5 / 2;\n";
    env::Environment env = run(input);

    std::cout << std::get<int>(env.get("b")) << std::endl;

    return 0;
}