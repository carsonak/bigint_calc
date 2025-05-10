add_subdirectory(bigint)

target_sources(bigint PUBLIC xalloc.c)

target_include_directories(bigint PUBLIC headers)
