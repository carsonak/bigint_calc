link_libraries(bigint)

include_directories(${PROJECT_SOURCE_DIR}/src/bigint/number_parsing)

add_executable(test__numstr_alloc test__numstr_alloc.c test_main.c)
add_test(NAME test__numstr_alloc COMMAND test__numstr_alloc
                                         --failed-output-only
)

add_executable(test__str_to_numstr test__str_to_numstr.c test_main.c)
add_test(NAME test__str_to_numstr COMMAND test__str_to_numstr
                                          --failed-output-only
)

add_executable(test__numstr_to_bigint test__numstr_to_bigint.c test_main.c)
add_test(NAME test__numstr_to_bigint COMMAND test__numstr_to_bigint
                                             --failed-output-only
)

add_executable(test__anybase_to_bi test__anybase_to_bi.c test_main.c)
add_test(NAME test__anybase_to_bi COMMAND test__anybase_to_bi
                                          --failed-output-only
)

set_property(
    TEST test__numstr_alloc test__str_to_numstr test__numstr_to_bigint
         test__anybase_to_bi PROPERTY TIMEOUT 5
)
