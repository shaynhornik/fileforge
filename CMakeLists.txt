cmake_minimum_required(VERSION 3.16)
project(fileforge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(fileforge_core
    src/rename_rule.cpp
    src/scanner.cpp
    src/test_fixture.cpp
)

target_include_directories(fileforge_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(fileforge_core PRIVATE -Wall -Wextra)

add_executable(fileforge
    src/main.cpp
)

target_link_libraries(fileforge PRIVATE fileforge_core)

enable_testing()

add_executable(fileforge_integration_test
    tests/integration_test.cpp
)

target_link_libraries(fileforge_integration_test PRIVATE fileforge_core)
add_test(NAME fileforge_integration_test COMMAND fileforge_integration_test)
