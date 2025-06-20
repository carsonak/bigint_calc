cmake_minimum_required(VERSION 3.27)

project(
    bigint_calc
    DESCRIPTION "Simple arbitrary precision calculator"
    LANGUAGES C
)

include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_available)
if(LTO_available)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION
        "$<NOT:$<CONFIG:Debug>>"
        CACHE BOOL "Enable Interprocedural Optimisations for non-debug builds."
    )
endif()

# Compiler warnings
set(CMAKE_C_STANDARD 17)
add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Wformat=2
    -Wshadow
    "$<$<C_COMPILER_ID:GNU>:-Wduplicated-branches>"
    "$<$<C_COMPILER_ID:GNU>:-Wduplicated-cond>"
    -Werror
    "$<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address,undefined>"
    "$<$<AND:$<C_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:-Og>"
)
add_link_options(
    "$<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address,undefined>"
)

add_subdirectory(src)

# Tests are automatically built if this is the main project
option(BIGINT_CALC_ENABLE_TESTING "Enable tests for bigint_calc"
       ${PROJECT_IS_TOP_LEVEL}
)

if(BIGINT_CALC_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
