target_sources(bigint PRIVATE _bigint_internals.c _numstr_internals.c _numstr_print.c)
target_include_directories(bigint PRIVATE ${CMAKE_CURRENT_LIST_DIR})
