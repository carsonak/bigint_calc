include_directories(${PROJECT_SOURCE_DIR}/src/bigint/internals)

set_directory_properties(PROPERTIES LABELS "test_bigint")

add_subdirectory(test_internals)
add_subdirectory(test_math)
add_subdirectory(test_number_parsing)

add_executable(test_bi_compare test_bi_compare.c test_main.c)
target_link_libraries(test_bi_compare PRIVATE bigint)
add_test(NAME test_bi_compare COMMAND test_bi_compare --failed-output-only)

set_property(TEST test_bi_compare PROPERTY TIMEOUT 5)
