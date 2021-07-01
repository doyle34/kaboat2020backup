#include <stdio.h>

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

geometry_msgs::Twist Twist_msg;

void joyCallBack(const sensor_msgs::Joy joy_data)
{
    
    if(joy_data.buttons[0]>0)
    {
        geometry_msgs::Twist Twist_msg;
        Twist_msg.angular.z = -joy_data.axes[0]/2.0;
        Twist_msg.linear.x = (-joy_data.axes[4]+0.5)/2.0;
        ros::NodeHandle r;
        ros::Publisher chatter_pub = r.advertise<geometry_msgs::Twist>("cmd_vel", 3);
        chatter_pub.publish(Twist_msg);
        ROS_INFO("%f\t%f", Twist_msg.angular.z,Twist_msg.linear.x);
    }


}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_to_twist");
  ROS_INFO("Starting ... JOY node");
  


  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("joy", 3, joyCallBack);

  ros::spin();

  return 0;
}