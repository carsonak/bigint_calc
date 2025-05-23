add_executable(time_bi_multiply_long time_bi_multiply.c)
target_compile_definitions(time_bi_multiply_long PRIVATE LONG_MULTIPLY)
target_link_libraries(time_bi_multiply_long PRIVATE bigint)
add_test(NAME time_bi_multiply_long COMMAND time_bi_multiply_long)

add_executable(time_bi_multiply_karatsuba time_bi_multiply.c)
target_link_libraries(time_bi_multiply_karatsuba PRIVATE bigint)
add_test(NAME time_bi_multiply_karatsuba COMMAND time_bi_multiply_karatsuba)

set_property(
    TEST time_bi_multiply_karatsuba time_bi_multiply_long
    PROPERTY LABELS "performance"
)
set_property(
    TEST time_bi_multiply_karatsuba time_bi_multiply_long PROPERTY TIMEOUT 30
)
set_property(
    TEST time_bi_multiply_karatsuba time_bi_multiply_long
    PROPERTY WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
