add_executable(test__bi_internals test__bi_internals.c test_main.c)
target_link_libraries(test__bi_internals PRIVATE bigint)
add_test(NAME test__bi_internals COMMAND test__bi_internals
                                         --failed-output-only
)

# Custom library with mocked out alloc functions.
add_library(bigint_with_dummy_xalloc dummy_xalloc.c)
target_link_libraries(bigint_with_dummy_xalloc PUBLIC bigint_common)

add_executable(test_bi_alloc_fail test_bi_alloc_fail.c test_main.c)
target_link_libraries(test_bi_alloc_fail PRIVATE bigint_with_dummy_xalloc)
add_test(NAME test_bi_alloc_fail COMMAND test_bi_alloc_fail
                                         --failed-output-only
)

set_property(TEST test_bi_alloc_fail test__bi_internals PROPERTY TIMEOUT 5)
