target_sources(reader PRIVATE reader.c)
target_link_libraries(reader PRIVATE readline string_type)

target_include_directories(
    reader
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE /usr/include
)
