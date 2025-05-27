add_executable(test_deque test_deque.c test_main.c)
add_test(NAME test_deque COMMAND test_deque --failed-output-only)

add_executable(test_linked_node test_linked_node.c test_main.c)
add_test(NAME test_linked_node COMMAND test_linked_node --failed-output-only)

set_property(TEST test_linked_node test_deque PROPERTY TIMEOUT 5)
