target_sources(bigint PRIVATE _numstr_alloc.c _numstr_print.c _numstr_to_bigint.c _str_to_numstr.c bi_tostr.c)
target_include_directories(bigint PRIVATE ${CMAKE_CURRENT_LIST_DIR})
