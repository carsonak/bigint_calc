target_sources(parser PRIVATE lexer.c lexer_token_tostr.c)
target_link_libraries(parser PRIVATE string_type)
target_include_directories(parser PRIVATE ${CMAKE_CURRENT_LIST_DIR})
