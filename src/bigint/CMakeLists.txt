add_library(bigint)

add_subdirectory(internals)
add_subdirectory(math)
add_subdirectory(parse_number)

target_sources(bigint PUBLIC bi_compare.c bi_delete.c bi_dup.c bi_new.c safe_imaxabs.c)
target_include_directories(bigint PUBLIC ${CMAKE_CURRENT_LIST_DIR})
