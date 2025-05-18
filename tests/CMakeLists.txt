include_directories(${PROJECT_SOURCE_DIR}/tests ${PROJECT_SOURCE_DIR}/tau
                    ${PROJECT_SOURCE_DIR}/src/bigint/internals)

add_subdirectory(test_bigint)
add_subdirectory(test_performance)
