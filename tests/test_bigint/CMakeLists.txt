link_libraries(bigint)

add_subdirectory(test_internals)
add_subdirectory(test_math)
add_subdirectory(test_parse_number)

add_executable(test_bi_compare test_bi_compare.c test_main.c)
add_test(NAME test_bi_compare COMMAND test_bi_compare --failed-output-only)

set_property(TEST test_bi_compare PROPERTY TIMEOUT 5)
