#include <stdio.h>
#include <stdlib.h>
#include <ros/ros.h>
#include <wiringPi.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Twist.h>

#define Trig 4
#define Echo 5

using namespace std;

class Ultra
{
public:
  Ultra()
  {
    wiringPiSetup();

    pubCmdvel = nh.advertise<geometry_msgs::Twist>("/cmdVel", 5);
    pubArrivalUltraSonicForward = nh.advertise<std_msgs::Int16>("/ultra_forward_arrival", 1);
    pubArrivalUltraSonicBackward = nh.advertise<std_msgs::Int16>("/ultra_backward_arrival", 1);

    subCheckUltraSonic = nh.subscribe("/ultra", 1, &Ultra::subUltraSonic, this);

    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
  }

  void robotStop()
  {
    ROS_INFO("STOP");
    cmdVel.linear.x = 0.0;
    pubCmdvel.publish(cmdVel);
    finish.data = 2;
    pubArrivalUltraSonicForward.publish(finish);
  }

  void robotStop2()
  {
    ROS_INFO("STOP");
    cmdVel.linear.x = 0.0;
    pubCmdvel.publish(cmdVel);


  }

  void robotGo()
  {
    cmdVel.linear.x = 0.15;
    pubCmdvel.publish(cmdVel);
  }

  void robotBack()
  {
    cmdVel.linear.x = -0.15;
    pubCmdvel.publish(cmdVel);
  }

  void robotUltra_forward()
  {
    int start_time, end_time;
    double distance;

    while (ros::ok())
    {
      digitalWrite(Trig, LOW);
      delay(1000);
      digitalWrite(Trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trig, LOW);

      while (digitalRead(Echo) == 0);
      start_time = micros();
      while (digitalRead(Echo) == 1);
      end_time = micros();
      distance = (end_time - start_time) / 29. / 2.;
      if (distance > 10)
      {
        printf("distance: %.2f cm\n", distance);
        this->robotGo();
      }
      else
      {
        this->robotStop();
        delayMicroseconds(10);
        break;
      }
    }
  }

  void robotUltra_backward()
  {
    int start_time, end_time;
    double distance;

    while (ros::ok())
    {
      digitalWrite(Trig, LOW);
      delay(1000);
      digitalWrite(Trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trig, LOW);

      while (digitalRead(Echo) == 0);
      start_time = micros();
      while (digitalRead(Echo) == 1);
      end_time = micros();
      distance = (end_time - start_time) / 29. / 2.;
      if (distance < 15)
      {
        printf("distance: %.2f cm\n", distance);
        this->robotBack();
      }
      else
      {
        this->robotStop2();
        delayMicroseconds(10);
        break;
      }
    }
  }

  void subUltraSonic(const std_msgs::Int16 subUltraSonic)
  {
    switch (subUltraSonic.data)
    {
    case 1:
      ROS_INFO("Forward Received!!");
      ROS_INFO("UltraSensor is Activating");
      robotUltra_forward();
      break;
    case 2:
      ROS_INFO("Backward Received!!");
      ROS_INFO("UltraSensor is Activating");
      robotUltra_backward();
      break;
    }
  }

private:
  ros::NodeHandle nh;

  ros::Publisher pubCmdvel;
  ros::Publisher pubArrivalUltraSonicForward;
  ros::Publisher pubArrivalUltraSonicBackward;

  ros::Subscriber subCheckUltraSonic;

  geometry_msgs::Twist cmdVel;
  std_msgs::Int16 finish;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "UltraSensor");

  Ultra ultra;

  ROS_INFO("Waiting for robot...");

  ros::spin();
  return 0;
}
