add_subdirectory(internals)

target_sources(parser PRIVATE parser.c)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
