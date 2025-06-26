target_sources(deque PRIVATE list_node.c deque.c)
target_include_directories(deque PUBLIC ${CMAKE_CURRENT_LIST_DIR})
