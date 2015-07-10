cmake_minimum_required(VERSION 2.8)
project( gen_features )
find_package( OpenCV REQUIRED )
find_package( Boost 1.53 COMPONENTS filesystem REQUIRED) 

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
ADD_DEFINITIONS(
  -g
  -O0
  -Wall
  -std=c++11
  )

include_directories(${Boost_INCLUDE_DIRS}) 
add_executable( 
  gen_features gen_features.cpp 
  )
target_link_libraries( gen_features ${OpenCV_LIBS} ${Boost_LIBRARIES} -lboost_system)

add_executable( 
  train train.cpp 
  )
target_link_libraries( train ${OpenCV_LIBS} ${Boost_LIBRARIES} -lboost_system)
