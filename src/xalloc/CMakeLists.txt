target_sources(bigint PRIVATE xalloc.c)
target_sources(data_structures PRIVATE xalloc.c)

target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(data_structures PUBLIC ${CMAKE_CURRENT_LIST_DIR})
