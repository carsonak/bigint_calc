add_executable(time_bi_multiply_naive time_bi_multiply.c)
target_link_libraries(time_bi_multiply_naive PRIVATE bigint)
add_test(NAME time_bi_multiply_naive COMMAND time_bi_multiply_naive)

add_executable(time_bi_multiply_karatsuba time_bi_multiply.c)
target_link_libraries(time_bi_multiply_karatsuba PRIVATE bigint)
target_compile_definitions(
    time_bi_multiply_karatsuba PRIVATE KARATSUBA_MULTIPLY
)
add_test(NAME time_bi_multiply_karatsuba COMMAND time_bi_multiply_karatsuba)

set_property(
    TEST time_bi_multiply_karatsuba time_bi_multiply_naive
    PROPERTY LABELS "performance"
)
set_property(
    TEST time_bi_multiply_karatsuba time_bi_multiply_naive PROPERTY TIMEOUT 10
)
