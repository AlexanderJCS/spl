cmake_minimum_required(VERSION 3.27)
project(spl)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(google_tests)

add_executable(spl main.cpp
        interpreter/tokenizer.cpp
        interpreter/tokenizer.h
        interpreter/rpn.cpp
        interpreter/rpn.h
        interpreter/parser.cpp
        interpreter/parser.h
        interpreter/ast.cpp
        interpreter/ast.h
        interpreter/environment.cpp
        interpreter/environment.h
        spl.cpp
        spl.h
)
