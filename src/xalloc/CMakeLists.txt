target_sources(bigint PRIVATE xalloc.c)
target_sources(deque PRIVATE xalloc.c)
target_sources(hash_table PRIVATE xalloc.c)
target_sources(string_type PRIVATE xalloc.c)

target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(deque PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(string_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})
