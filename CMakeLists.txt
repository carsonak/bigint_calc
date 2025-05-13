cmake_minimum_required(VERSION 3.27)

project(
    bigint_calc
    DESCRIPTION "Simple arbitrary precision calculator"
    LANGUAGES C)

# Compiler warnings
set(CMAKE_C_STANDARD 17)
add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Wformat=2
    -Wshadow
    -Wduplicated-branches
    -Wduplicated-cond
    -Werror
    "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
add_link_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")

add_subdirectory(src)

# Tests are automatically built if this is the main project
option(BIGINT_CALC_ENABLE_TESTING "Enable tests for bigint_calc"
       ${PROJECT_IS_TOP_LEVEL})

if(BIGINT_CALC_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
