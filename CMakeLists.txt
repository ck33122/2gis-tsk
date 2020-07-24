cmake_minimum_required(VERSION 3.1)

set(SOURCES
  # ${CMAKE_CURRENT_SOURCE_DIR}/src/ todo
)
set(TESTS
  ${SOURCES}
  # ${CMAKE_CURRENT_SOURCE_DIR}/tests/ todo
)

#######################################################
project(task CXX)
add_executable(task ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${SOURCES})
target_include_directories(task PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(task PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

#######################################################
project(tests CXX)
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp ${TESTS})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(tests PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)