add_library(bigint_common OBJECT)
add_library(bigint)

add_subdirectory(bigint)
add_subdirectory(data_structures)
add_subdirectory(xalloc)

target_include_directories(bigint_common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(data_structures PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(bigint PUBLIC bigint_common)
