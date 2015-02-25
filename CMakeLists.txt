cmake_minimum_required(VERSION 2.8)
project(SloMo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_INSTALL_PREFIX /home/sourabhd/projects/slomo/build)
set(OpenCV_DIR /home/sourabhd/projects/slomo/thirdparty/opencv/build)

find_package(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS} )


#add_executable( fback src/fback.cpp )
add_executable(tvl1 src/tvl1_optical_flow.cpp)
#target_link_libraries( DisplayImage ${OpenCV_LIBS} )
#target_link_libraries( fback ${OpenCV_LIBS} )
target_link_libraries(tvl1 ${OpenCV_LIBS})

set(SloMo_SOURCES src/main.cpp src/SloMo.cpp src/delaunay2.cpp src/synthetic.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -fopenmp")
add_executable(slomo ${SloMo_SOURCES})
target_link_libraries(slomo ${OpenCV_LIBS} )
set(TARGET slomo CXX_STANDARD 11)
