add_library(parser STATIC)

add_subdirectory(internals)

target_sources(parser PRIVATE parser.c)
target_link_libraries(parser PRIVATE String_type deque)

target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
