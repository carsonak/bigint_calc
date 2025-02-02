cmake_minimum_required(VERSION 3.21)

project(bigint_calc
    DESCRIPTION "Simple arbitrary number calculator"
    LANGUAGES C
)

add_subdirectory(src)

option(BIGINT_CALC_ENABLE_TESTING "Enable tests for bigint_calc" ${PROJECT_IS_TOP_LEVEL})

if(BIGINT_CALC_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
