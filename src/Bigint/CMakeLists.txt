add_library(Bigint_common OBJECT)
add_library(Bigint)

add_subdirectory(internals)
add_subdirectory(math)
add_subdirectory(number_parsing)

target_sources(
    Bigint_common PRIVATE bi_compare.c bi_delete.c bi_dup.c bi_new.c bi_tostr.c
)

target_include_directories(Bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
