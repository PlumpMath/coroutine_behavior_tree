CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(cbt)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
find_package(Boost 1.54 REQUIRED COMPONENTS coroutine context)
include_directories(include ${BOOST_INCLUDE_DIRS})
add_library(cbt src/cbt.cpp)
target_link_libraries(cbt ${Boost_LIBRARIES})
add_executable(test_cbt src/test_cbt.cpp)
target_link_libraries(test_cbt cbt ${Boost_LIBRARIES})