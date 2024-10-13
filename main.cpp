#include <string>
#include "spl.h"
#include "interpreter/ast.h"

int main() {
    std::string input = "fun x(a, b) {\n"
                        "    c = b;\n"
                        "    d = a;\n"
                        "    e = a + b;\n"
                        "}\n"
                        "\n"
                        "x(3, 2);";
    env::Environment env = run(input);

    return 0;
}