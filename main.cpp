#include <iostream>

#include "spl.h"

int main() {
    std::string input = "fun a() {b = 3;} a();";
    env::Environment env = run(input);

    std::cout << "x type: " << env.getType("x") << " x value: " << std::get<int>(env.get("x")) << std::endl;

    return 0;
}
