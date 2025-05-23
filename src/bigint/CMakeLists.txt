add_subdirectory(internals)
add_subdirectory(math)
add_subdirectory(number_parsing)

target_sources(bigint_common PUBLIC bi_compare.c bi_delete.c bi_dup.c bi_new.c bi_print.c)
target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
