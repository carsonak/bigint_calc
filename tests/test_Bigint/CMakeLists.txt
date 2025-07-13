include_directories(${PROJECT_SOURCE_DIR}/src/Bigint/internals)

set_directory_properties(PROPERTIES LABELS "test_Bigint")

add_subdirectory(test_internals)
add_subdirectory(test_math)
add_subdirectory(test_number_parsing)

add_executable(test_bi_compare test_bi_compare.c test_main.c)
target_link_libraries(test_bi_compare PRIVATE Bigint)
add_test(NAME test_bi_compare COMMAND test_bi_compare --failed-output-only)

add_executable(test_bi_tostr test_bi_tostr.c test_main.c)
target_link_libraries(test_bi_tostr PRIVATE Bigint)
add_test(NAME test_bi_tostr COMMAND test_bi_tostr --failed-output-only)

set_property(TEST test_bi_compare PROPERTY TIMEOUT 5)
