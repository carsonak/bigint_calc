target_sources(bigint PUBLIC numstr_to_bigint.c str_to_numstr.c bi_tostr.c)
target_include_directories(bigint PUBLIC ${CMAKE_CURRENT_LIST_DIR})
