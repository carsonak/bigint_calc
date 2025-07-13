add_library(String_type STATIC String.c)

target_include_directories(String_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})
