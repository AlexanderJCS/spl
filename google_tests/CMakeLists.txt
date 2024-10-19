# 'Google_test' is the subproject name
project(google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# Include all test source files and relevant project files in a single add_executable call
add_executable(Google_Tests_run
        ../interpreter/tokenizer.cpp
        ../interpreter/tokenizer.h
        ../interpreter/rpn.cpp
        ../interpreter/rpn.h
        ../interpreter/parser.cpp
        ../interpreter/parser.h
        ../interpreter/ast.cpp
        ../interpreter/ast.h
        ../interpreter/environment.cpp
        ../interpreter/environment.h
        ../interpreter/control_flow.cpp
        ../interpreter/control_flow.h
        ../spl.cpp
        ../spl.h
        test_operators.cpp
        test_tokenizer.cpp
)

# Link with Google Test libraries
target_link_libraries(Google_Tests_run gtest gtest_main)

# Enable CTest to integrate with CMake's testing functionality
enable_testing()

# Register the test with CTest
add_test(NAME GoogleTestRun COMMAND Google_Tests_run)
