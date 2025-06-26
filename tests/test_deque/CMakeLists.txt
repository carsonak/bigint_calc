link_libraries(deque)

set_directory_properties(PROPERTIES LABELS "test_deque")

add_executable(test_deque test_deque.c test_main.c)
add_test(NAME test_deque COMMAND test_deque --failed-output-only)

add_executable(test_list_node test_list_node.c test_main.c)
add_test(NAME test_list_node COMMAND test_list_node --failed-output-only)

set_property(TEST test_list_node test_deque PROPERTY TIMEOUT 5)
