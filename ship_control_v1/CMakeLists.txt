cmake_minimum_required(VERSION 3.0.2)
project(ship_control_v1)

find_package(catkin REQUIRED COMPONENTS
roscpp
rospy
std_msgs
message_generation
)


add_message_files(
   FILES
   Ship_status_indicator.msg
)

 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )

catkin_package(

)


include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)


#add_executable(indicator_node src/indicator_node.cpp)
#target_link_libraries(indicator_node ${catkin_LIBRARIES})
#add_dependencies(indicator_node ship_control_v1_generate_messages_cpp)

add_executable(control_node src/control_node.cpp)
target_link_libraries(control_node ${catkin_LIBRARIES})
add_dependencies(control_node ship_control_v1_generate_messages_cpp)


add_executable(joy_to_twist src/joy_to_twist.cpp)
target_link_libraries(joy_to_twist ${catkin_LIBRARIES})
add_dependencies(joy_to_twist ship_control_v1_generate_messages_cpp)

FIND_LIBRARY(EXT_LIBRARY pigpio ~/ck_lee/src/pigpio_master)

#target_link_libraries(
  #indicator_node
  #${catkin_LIBRARIES} ${EXT_LIBRARY}
#)
target_link_libraries(
  control_node
  ${catkin_LIBRARIES} ${EXT_LIBRARY}
)

find_package(catkin REQUIRED COMPONENTS roslaunch)
roslaunch_add_file_check(launch)