include_directories(
    ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SOURCE_DIR}/tau
    ${PROJECT_SOURCE_DIR}/src/bigint/internals
    ${PROJECT_SOURCE_DIR}/src/bigint/number_parsing
)

add_subdirectory(test_bigint)
add_subdirectory(test_deque)
add_subdirectory(test_performance)
