add_subdirectory(Bigint)
add_subdirectory(Deque)
add_subdirectory(hash_table)
add_subdirectory(parser)
add_subdirectory(reader)
add_subdirectory("String")
add_subdirectory(xalloc)

target_include_directories(Bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Deque PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(reader PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(String_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Bigint PUBLIC Bigint_common)

target_sources(calc PRIVATE main.c)
target_link_libraries(calc PRIVATE Bigint Deque hash_table parser reader String_type)
