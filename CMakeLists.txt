cmake_minimum_required (VERSION 3.9)
project (Utils)

# The version number.
set (Utils_VERSION_MAJOR 1)
set (Utils_VERSION_MINOR 0)

# set the c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # needed for googletest to compile in gcc

# add the binary tree to the search path for include files
# so that we will find Project1Config.h
include_directories ("${PROJECT_BINARY_DIR}")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

set (LIB_SRCS 
	lib/include/stack.h 
	lib/src/stack.cpp 
	lib/include/list.h 
	lib/src/list.cpp 
	lib/include/queue.h 
	lib/src/queue.cpp 
	lib/include/bst.h 
	lib/src/bst.cpp)
	
set (TEST_SRCS 
	test/src/stack_emplace_push_copy_test.cpp
	test/src/stack_emplace_push_move_test.cpp
	test/src/stack_copy_constructor_test.cpp
	test/src/stack_copy_operator_test.cpp
	test/src/stack_move_constructor_test.cpp
	test/src/stack_move_operator_test.cpp
	test/src/stack_pop_test.cpp
	test/src/stack_push_test.cpp
	test/src/stack_resize_test.cpp)

# add library
add_library (UtilsLib ${LIB_SRCS})

# add the executable
add_executable (UtilsMain proj/src/main.cpp)
target_link_libraries (UtilsMain UtilsLib)

# add test project
add_executable (UtilsTests ${TEST_SRCS})
target_link_libraries (UtilsTests UtilsLib)
target_link_libraries (UtilsTests gtest_main)

#
#
#   INSTALL
#
#

# add the install targets
#install (TARGETS Utils DESTINATION bin)

#
#
#   TESTS
#
#
include (CTest)
add_test (NAME unit COMMAND ${CMAKE_BINARY_DIR}/UnitTests)


