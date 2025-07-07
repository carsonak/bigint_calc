add_library(string_type STATIC string_ty.c)

target_include_directories(string_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})
