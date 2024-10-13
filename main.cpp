#include <string>
#include "spl.h"
#include "interpreter/ast.h"

int main() {
    // todo: function calls need a comma at the end of the argument list so the ) token doesn't go into the RPN
    //  parser. This is currently intended behavior but instead should be changed to allow for no comma at the end of
    //  the argument list.
    std::string input = "fun x(a, b) {\n"
                        "    c = b;\n"
                        "    d = a;\n"
                        "    return a;\n"
                        "}\n"
                        "\n"
                        "x(3, 2,);";
    env::Environment env = run(input);

    return 0;
}