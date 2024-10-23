#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = "a = 0;"
                        "while (!(a == 10)) {"
                        "    if (a == 9) {"
                        "        a = a + 2;"
                        "        continue;"
                        "    }"
                        "    a = a + 1;"
                        "    if (a == 5) {"
                        "        break;"
                        "    }"
                        "}";
    env::Environment env = run(input);

    std::cout << std::get<int>(env.get("a")) << std::endl;

    return 0;
}