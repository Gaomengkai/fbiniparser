cmake_minimum_required(VERSION 3.15)

project(INIDOC)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Input files:
# ast.hpp inidoc.cpp inidoc.hpp lex.yy.cpp parser.tab.cpp parser.tab.hpp
# Output files:
# inidoc.a
add_library(inidoc STATIC ast.hpp inidoc.cpp inidoc.hpp lex.yy.cpp parser.tab.cpp parser.tab.hpp)

# Input files:
# inidoc.hpp main.cpp
# Input libraries:
# inidoc
# Output files:
# inidoc_main
add_executable(inidoc_main inidoc.hpp main.cpp)
target_link_libraries(inidoc_main inidoc)