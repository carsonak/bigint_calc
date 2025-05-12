add_executable(test_bi_add test_bi_add.c test_main.c)
target_link_libraries(test_bi_add PRIVATE bigint)
add_test(NAME test_bi_add COMMAND test_bi_add)

add_executable(test_bi_iadd test_bi_iadd.c test_main.c)
target_link_libraries(test_bi_iadd PRIVATE bigint)
add_test(NAME test_bi_iadd COMMAND test_bi_iadd)

add_executable(test_bi_sub test_bi_sub.c test_main.c)
target_link_libraries(test_bi_sub PRIVATE bigint)
add_test(NAME test_bi_sub COMMAND test_bi_sub)

add_executable(test_bi_isub test_bi_isub.c test_main.c)
target_link_libraries(test_bi_isub PRIVATE bigint)
add_test(NAME test_bi_isub COMMAND test_bi_isub)

add_executable(test_bi_mul test_bi_mul.c test_main.c)
target_link_libraries(test_bi_mul PRIVATE bigint)
add_test(NAME test_bi_mul COMMAND test_bi_mul)

add_executable(test_bi_div test_bi_div.c test_main.c)
target_link_libraries(test_bi_div PRIVATE bigint)
add_test(NAME test_bi_div COMMAND test_bi_div)

add_executable(test_bi_pow test_bi_pow.c test_main.c)
target_link_libraries(test_bi_pow PRIVATE bigint)
add_test(NAME test_bi_pow COMMAND test_bi_pow)

add_executable(test_bi_iszero test_bi_iszero.c test_main.c)
target_link_libraries(test_bi_iszero PRIVATE bigint)
add_test(NAME test_bi_iszero COMMAND test_bi_iszero)
