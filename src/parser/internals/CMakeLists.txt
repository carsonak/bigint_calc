target_sources(parser PRIVATE lexer.c lexer_token.c)

target_link_libraries(parser PRIVATE reader)

target_include_directories(parser PRIVATE ${CMAKE_CURRENT_LIST_DIR})
