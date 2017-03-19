cmake_minimum_required(VERSION 2.8.3)
project(grabby_mcarmface_ros)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

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
  button.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_executable(serialNode src/serialNode.cpp)
add_dependencies(serialNode ${PROJECT_NAME}_EXPORTED_LIBRARIES  ${grabby_mcarmface_ros_EXPORTED_TARGETS})
target_link_libraries(serialNode ${catkin_LIBRARIES} ${serial_LIBRARIES} ${serial_utils_LIBRARIES})
