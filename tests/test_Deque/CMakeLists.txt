link_libraries(Deque)

set_directory_properties(PROPERTIES LABELS "test_Deque")

add_executable(test_Deque Deque.c test_Deque.c test_main.c)
add_test(NAME test_Deque COMMAND test_Deque --failed-output-only)

add_executable(test_ListNode ListNode.c test_ListNode.c test_main.c)
add_test(NAME test_ListNode COMMAND test_ListNode --failed-output-only)

set_property(TEST test_ListNode test_Deque PROPERTY TIMEOUT 5)
