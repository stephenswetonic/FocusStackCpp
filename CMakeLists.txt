cmake_minimum_required(VERSION 3.9)
set(CMAKE_OSX_ARCHITECTURES x86_64)
project(opencvtest)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(Eigen_INCLUDE_DIR "/Users/stephenswetonic/Documents/Eigen/eigen-3.4.0/Eigen/")
include_directories(${Eigen_INCLUDE_DIR})


add_executable(opencvtest main.cpp BlurMap.cpp BlurMap.h)

target_link_libraries(opencvtest ${OpenCV_LIBS})

