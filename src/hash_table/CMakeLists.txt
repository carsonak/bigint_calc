add_library(hash_table STATIC hash_table.c)
target_include_directories(hash_table PUBLIC ${CMAKE_CURRENT_LIST_DIR})
