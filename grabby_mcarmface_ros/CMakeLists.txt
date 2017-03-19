cmake_minimum_required(VERSION 2.8.3)
project(grabby_mcarmface_ros)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  serial
  serial_utils
  std_msgs
  message_generation
)

add_message_files(
  FILES
  ServoAngleArray.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)
###################################
## catkin specific configuration ##
###################################
catkin_package(
    CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_executable(serialNode src/serialNode.cpp)
add_dependencies(serialNode ${PROJECT_NAME}_EXPORTED_LIBRARIES  ${grabby_mcarmface_ros_EXPORTED_TARGETS})
target_link_libraries(serialNode ${catkin_LIBRARIES} ${serial_LIBRARIES} ${serial_utils_LIBRARIES})
