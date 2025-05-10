add_executable(test_bi_add test_bi_add.c test_main.c)
target_link_libraries(test_bi_add PRIVATE bigint)
add_test(NAME test_bi_add COMMAND test_bi_add)
