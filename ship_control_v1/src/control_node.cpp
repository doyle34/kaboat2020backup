#include <stdio.h>
#include <pigpio.h>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include <cmath>
#include <sstream>

// Pin settings
#define SV_L_PIN 3
#define SV_R_PIN 4 

#define TH_L_PIN 17
#define TH_R_PIN 18

// Servo MIN MAX RANGE
#define SV_L_MID 1500
#define SV_L_MIN 1200
#define SV_L_MAX 1700

#define SV_R_MID 1500
#define SV_R_MIN 1200
#define SV_R_MAX 1700

#define TH_L_MID 1500
#define TH_R_MID 1500

#define TH_CONST 300
#define SV_CONST 300

#define TH_L_MAX 1800
#define TH_R_MAX 1800

#define TH_L_MIN 1500
#define TH_R_MAX 1500


void set_l_servo(float pwm)
{
  if(pwm>SV_L_MAX)
    gpioPWM(SV_L_PIN, SV_L_MAX);
  else if(pwm<SV_L_MIN)
    gpioPWM(SV_L_PIN, SV_L_MIN);
  else
    gpioPWM(SV_L_PIN, pwm);
}
void set_r_servo(float pwm)
{
  if(pwm>SV_R_MAX)
    gpioPWM(SV_R_PIN, SV_R_MAX);
  else if(pwm<SV_R_MIN)
    gpioPWM(SV_R_PIN, SV_R_MIN);
  else
    gpioPWM(SV_R_PIN, pwm);
}
void set_l_thrust(float pwm)
{
  if(pwm>TH_L_MAX)
    gpioPWM(TH_L_PIN, TH_R_MAX);
  else if(pwm<TH_L_MIN)
    gpioPWM(TH_L_PIN, TH_L_MIN);
  else
    gpioPWM(TH_L_PIN, pwm);
}
void set_r_thrust(float pwm)
{
  if(pwm>TH_R_MAX)
    gpioPWM(TH_R_PIN, TH_R_MAX);
  else if(pwm<TH_R_MIN)
    gpioPWM(TH_R_PIN, TH_R_MIN);
  else
    gpioPWM(TH_R_PIN, pwm);
}
void servo_thruster_GPIO(const geometry_msgs::Twist ship_twist)
{
    if(abs(ship_twist.angular.z)<0.25){
      set_l_servo(ship_twist.angular.z*SV_CONST);
      set_r_servo(ship_twist.angular.z*SV_CONST);

      set_l_thrust(TH_L_MID+ship_twist.linear.x*TH_CONST);
      set_r_thrust(TH_R_MID+ship_twist.linear.x*TH_CONST);
    }else{
      set_l_servo(ship_twist.angular.z*SV_CONST);
      set_r_servo(ship_twist.angular.z*SV_CONST);

      set_l_thrust(TH_L_MID+ship_twist.linear.x*TH_CONST-ship_twist.angular.z*TH_CONST);
      set_r_thrust(TH_R_MID+ship_twist.linear.x*TH_CONST+ship_twist.angular.z*TH_CONST);
    }

  ROS_INFO("Changed to ... angular: %f\tlinear: %f", ship_twist.angular.z, ship_twist.linear.x);

}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "control_node");
  ROS_INFO("Starting ... control node");
  ros::NodeHandle n;
  ROS_INFO("LED Configuration...");
 
  ROS_INFO("Initializing PiGPIO...");

  if (gpioInitialise() < 0){ROS_INFO("PiGPIO init Failed please do:\t sudo killall pigpiod");} //pigpio initialization
  else
  {
    ROS_INFO("PiGPIO init SUCCESS");

    gpioSetPWMrange(SV_L_PIN, 20000);
    gpioSetPWMrange(SV_R_PIN, 20000);
    gpioSetPWMrange(TH_L_PIN, 20000);
    gpioSetPWMrange(TH_R_PIN, 20000);

    gpioSetPWMfrequency(SV_L_PIN, 50);
    gpioSetPWMfrequency(SV_R_PIN, 50);
    gpioSetPWMfrequency(TH_L_PIN, 50);
    gpioSetPWMfrequency(TH_R_PIN, 50);

    gpioPWM(SV_L_PIN, SV_L_MID);
    gpioPWM(SV_R_PIN, SV_R_MID);
    gpioPWM(TH_L_PIN, TH_L_MID);
    gpioPWM(TH_R_PIN, TH_R_MID);
  }
  ros::Subscriber sub = n.subscribe("cmd_vel", 1, servo_thruster_GPIO);

  ros::spin();

  return 0;
}
