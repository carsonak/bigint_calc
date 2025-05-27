add_subdirectory(test_internals)

add_executable(test_lexer test_lexer.c test_main.c)
add_test(NAME test_lexer COMMAND test_lexer --failed-output-only)

set_property(TEST test_lexer PROPERTY TIMEOUT 5)
