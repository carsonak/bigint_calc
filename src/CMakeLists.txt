add_library(bigint_common OBJECT)
add_library(bigint)
add_library(deque)
add_library(hash_table)
add_library(string_type)

add_subdirectory(bigint)
add_subdirectory(deque)
add_subdirectory(hash_table)
add_subdirectory(string_type)

add_subdirectory(parser)
add_subdirectory(xalloc)

target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(deque PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(string_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(bigint PUBLIC bigint_common)
