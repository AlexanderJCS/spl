cmake_minimum_required(VERSION 3.27)
project(spl)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(google_tests)

add_executable(spl main.cpp
        token/tokenizer.cpp
        token/tokenizer.h
        ast/rpn.cpp
        ast/rpn.h
        ast/parser.cpp
        ast/parser.h
        ast/ast.cpp
        ast/ast.h
        ast/interpreter.cpp
        ast/interpreter.h
)
