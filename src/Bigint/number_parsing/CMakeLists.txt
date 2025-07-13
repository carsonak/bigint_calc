target_sources(
    Bigint_common PRIVATE _numstr_alloc.c _numstr_print.c _numstr_to_bigint.c
                          _numstr_new.c
)

target_include_directories(Bigint_common PRIVATE ${CMAKE_CURRENT_LIST_DIR})
