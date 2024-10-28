#include <string>
#include "spl.h"
#include "interpreter/ast.h"

#include <iostream>

int main() {
    std::string input = R"(a = "a" < "b";)";
    env::Environment env = run(input);

    std::cout << std::get<bool>(env.get("a")) << std::endl;

    return 0;
}