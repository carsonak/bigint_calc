cmake_minimum_required(VERSION 3.27)

project(
    bigint_calc
    DESCRIPTION "Simple arbitrary precision calculator"
    LANGUAGES C
)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

add_subdirectory(src)

# Tests are automatically built if this is the main project
option(BIGINT_CALC_ENABLE_TESTING "Enable tests for bigint_calc"
       ${PROJECT_IS_TOP_LEVEL})

if(BIGINT_CALC_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
