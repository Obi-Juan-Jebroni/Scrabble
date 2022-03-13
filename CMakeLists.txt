cmake_minimum_required(VERSION 3.5)
project(SCRABBLE CXX)

# require a C++11 compiler
# use it for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Board program
set(board_src
  board.cpp
  board.h
)

# Main scrabble program
set(scrabble_src
  ${board_src}
  WordSearch.cpp
)

# Test program
set(test_src
  ${board_src}
  test.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# create the scrabble executable
# add_executable(scrabble ${scrabble_src})

# create the test executable
add_executable(test ${test_src})