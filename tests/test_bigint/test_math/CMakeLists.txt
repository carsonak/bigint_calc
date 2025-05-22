add_executable(test_bi_add test_bi_add.c test_main.c)
add_test(NAME test_bi_add COMMAND test_bi_add --failed-output-only)

add_executable(test_bi_iadd test_bi_iadd.c test_main.c)
add_test(NAME test_bi_iadd COMMAND test_bi_iadd --failed-output-only)

add_executable(test_bi_sub test_bi_sub.c test_main.c)
add_test(NAME test_bi_sub COMMAND test_bi_sub --failed-output-only)

add_executable(test_bi_isub test_bi_isub.c test_main.c)
add_test(NAME test_bi_isub COMMAND test_bi_isub --failed-output-only)

add_executable(test_bi_mul test_bi_mul.c test_main.c)
add_test(NAME test_bi_mul COMMAND test_bi_mul --failed-output-only)

add_executable(test_bi_div test_bi_div.c test_main.c)
add_test(NAME test_bi_div COMMAND test_bi_div --failed-output-only)

add_executable(test_bi_pow test_bi_pow.c test_main.c)
add_test(NAME test_bi_pow COMMAND test_bi_pow --failed-output-only)

add_executable(test_bi_iszero test_bi_iszero.c test_main.c)
add_test(NAME test_bi_iszero COMMAND test_bi_iszero --failed-output-only)

add_executable(test_bi_shift test_bi_shift.c test_main.c)
add_test(NAME test_bi_shift COMMAND test_bi_shift --failed-output-only)

set_property(
    TEST test_bi_add
         test_bi_div
         test_bi_iadd
         test_bi_isub
         test_bi_iszero
         test_bi_mul
         test_bi_pow
         test_bi_shift
         test_bi_sub
    PROPERTY TIMEOUT 5
)
