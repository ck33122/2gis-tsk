cmake_minimum_required(VERSION 3.1)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/task/count_words.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/task/checksum.cpp)

set(TESTS ${SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/result.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/checksum.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/count_words.cpp)

#######################################################
project(task CXX)
add_executable(task ${CMAKE_CURRENT_SOURCE_DIR}/task/main.cpp ${SOURCES})
set_target_properties(task PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

#######################################################
project(tests CXX)
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp ${TESTS})
target_include_directories(tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/task
  ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set_target_properties(tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
