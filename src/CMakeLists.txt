cmake_minimum_required(VERSION 3.13)
add_library("cli" STATIC "cli.cpp")
set_property(TARGET "cli" PROPERTY CXX_STANDARD 17)
target_include_directories("cli" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
