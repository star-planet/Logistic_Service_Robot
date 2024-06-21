#include <stdio.h>
#include <ros/ros.h>
#include <std_msgs/Int16.h>

#include "stepMotor.h"

using namespace std;

class StepMotor
{
public:
  StepMotor()
  {
    pub = nh.advertise<std_msgs::Int16>("/motor_arrival", 1);
    sub = nh.subscribe("/motor", 1, &StepMotor::motorCallback, this);
  }

  void motorCallback(const std_msgs::Int16 msg)
  {
    switch (msg.data)
    {
    case 101:
      ROS_INFO("First floor is activating");
      forward1(11);
      break;

    case 102:
      ROS_INFO("Second floor is activating");
      forward2(96);
      break;

    case 103:
      ROS_INFO("Third floor is activating");
      forward3(96);
      break;
    }
    finish.data = 3;
    pub.publish(finish);
  }

private:
  ros::NodeHandle nh;

  ros::Publisher pub;
  ros::Subscriber sub;

  std::string name;
  std_msgs::Int16 finish;
  std_msgs::Int16 msg;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "step_motor");

  ros::NodeHandle nh;
  ROS_INFO("All MOTORS ON!");

  wiringPiSetupGpio();

  StepMotor stepMotor;

  ros::spin();

  return 0;
}
