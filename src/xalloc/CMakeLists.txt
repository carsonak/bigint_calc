# target_sources(bigint_common PUBLIC xalloc.c)
target_sources(bigint PUBLIC xalloc.c)
target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
