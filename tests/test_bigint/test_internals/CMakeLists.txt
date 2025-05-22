add_executable(test__bigint_internals test__bigint_internals.c test_main.c)
add_test(NAME test__bigint_internals COMMAND test__bigint_internals
                                             --failed-output-only
)

add_executable(test__numstr_internals test__numstr_internals.c test_main.c)
add_test(NAME test__numstr_internals COMMAND test__numstr_internals
                                             --failed-output-only
)

set_property(
    TEST test__bigint_internals test__numstr_internals PROPERTY TIMEOUT 5
)
