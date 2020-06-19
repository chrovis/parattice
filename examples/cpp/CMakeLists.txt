cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

project(parattice-example VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror -fPIC ${CMAKE_CXX_FLAGS}")
  set(PARATTICE_LIBRARIES parattice dl)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror -fPIC ${CMAKE_CXX_FLAGS}")
  set(PARATTICE_LIBRARIES parattice dl)
else()
  message(WARNING "parattice may not support the detected compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/../../bindings/cpp/include
  ${PROJECT_BINARY_DIR})

add_library(parattice SHARED IMPORTED)
set_property(
  TARGET parattice
  PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../../target/release/libparattice.a
  )
add_executable(parattice_example parattice_example.cc)
target_link_libraries(parattice_example parattice dl pthread)
