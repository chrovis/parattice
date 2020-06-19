if(PARATTICE_GTEST_SOURCE_DIR)
  include(ExternalProject)
  ExternalProject_Add(
    GTest
    SOURCE_DIR ${PARATTICE_GTEST_SOURCE_DIR}
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
  )
  add_subdirectory(
    ${PARATTICE_GTEST_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
  )
  set(GTEST_BOTH_LIBRARIES gtest gtest_main)
else()
  find_package(GTest REQUIRED)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GTEST_INCLUDE_DIRS}
)

function(parattice_test name)
  add_executable(${name}_test
    ${name}_test.cc)
  if(PARATTICE_GTEST_SOURCE_DIR)
    add_dependencies(${name}_test GTest)
  endif()
  target_link_libraries(${name}_test ${PARATTICE_LIBRARIES} ${GTEST_BOTH_LIBRARIES} pthread)
  add_test(
    NAME ${name}_test
    COMMAND ${name}_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endfunction()

parattice_test(parattice)
parattice_test(lattice_kmp)
