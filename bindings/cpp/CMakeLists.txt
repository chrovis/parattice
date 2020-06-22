cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

project(parattice VERSION 0.2.2 LANGUAGES CXX)

option(PARATTICE_BUILD_TESTS "Builds test binaries." OFF)
option(PARATTICE_GTEST_SOURCE_DIR "Source directory of Google Test library." "")

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
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR})

add_library(parattice STATIC IMPORTED)
set_property(
  TARGET parattice
  PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../../target/release/libparattice.a
  )

add_subdirectory(include)

if(PARATTICE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
