add_library(reader STATIC reader.c)
target_link_libraries(reader PRIVATE readline String_type)

target_include_directories(
    reader
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
