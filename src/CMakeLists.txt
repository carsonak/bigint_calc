add_subdirectory(bigint)
add_subdirectory(deque)
add_subdirectory(hash_table)
add_subdirectory(parser)
add_subdirectory(reader)
add_subdirectory(string_type)
add_subdirectory(xalloc)

target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(deque PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(reader PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(string_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(bigint PUBLIC bigint_common)

target_sources(calc PRIVATE main.c)
target_link_libraries(calc PRIVATE bigint deque hash_table parser reader string_type)
