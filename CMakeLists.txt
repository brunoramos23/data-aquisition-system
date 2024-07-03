cmake_minimum_required(VERSION 3.10)
project(data-aquisition-system)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_THREAD_PREFER_PTHREAD ON)                                                                                                                                                                                                           
set(THREADS_PREFER_PTHREAD_FLAG ON)                                                                                                                                                                                                           
find_package(Threads REQUIRED)                                                                                                                                                                                                                


# find Boost
find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

# include directories
include_directories(${Boost_INCLUDE_DIRS} src)

file(GLOB_RECURSE PROJ_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE PROJ_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

# target executable and its source files
add_executable(das ${PROJ_SRCS} ${PROJ_HDRS})
target_include_directories(das PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>src)

# link Boost libraries to the target executable
target_link_libraries(das ${Boost_LIBRARIES})
target_link_libraries(das  Threads::Threads)                                                                                                 