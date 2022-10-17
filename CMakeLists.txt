# CMakeLists.txt

cmake_minimum_required(VERSION 3.5)
project(lsd)

set(CMAKE_DEBUG_POSTFIX d)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)


#set(CMAKE_CXX_FLAGS "-w")
set(CMAKE_CXX_FLAGS "-Wno-write-strings")

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
  set(CMAKE_CXX_FLAGS "-D DEBUG")
endif()


find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package (OpenCV REQUIRED)


set(SOURCES 
   src/lsd.cpp 
   src/kmeans.c
   src/statistics.cpp
   include/statistics.h
   include/kmeans.h
   include/knncpp.h 
   include/lsd.hpp)


add_library(lsd SHARED ${SOURCES})
set_target_properties(lsd PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(lsd PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lsd)

target_include_directories(
  lsd PUBLIC
   include
   ${EIGEN3_INCLUDE_DIR}
   ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(lsd ${OpenCV_LIBS})

# LSD TEST
add_executable(lsdtest src/lsd_test.cpp)
set_target_properties(lsdtest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_include_directories(
  lsdtest PUBLIC
  ${SOURCES}
  ${SOURCES_KMEANS}
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(lsdtest ${OpenCV_LIBS} lsd)



