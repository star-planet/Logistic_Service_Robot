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
  pubCmdvel                    = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 5);
  pubArrivalUltraSonicForward  = nh.advertise<std_msgs::Int16>("/ultra_forward_arrival", 1);
  pubArrivalUltraSonicBackward = nh.advertise<std_msgs::Int16>("/ultra_backward_arrival", 1);

  subCheckUltraSonic           = nh.subscribe("/ultra", 1, &Ultra::subUltraSonic , this);
  }

  void robotStop()
  {
    ROS_INFO("STOP");
    cmd_vel.linear.x = 0.0;
    pubCmdvel.publish(cmd_vel);
    fin.data = 2;
    pubArrivalUltraSonicForward.publish(fin);
  }

  void robotStop2()
  {
    ROS_INFO("STOP");
    cmd_vel.linear.x = 0.0;
    pubCmdvel.publish(cmd_vel);
    fin2.data = 4;
    pubArrivalUltraSonicBackward.publish(fin2);
  }

  void robotGo()
  {
    cmd_vel.linear.x = 0.15;
    pubCmdvel.publish(cmd_vel);
  }

  void robotBack()
  {
    cmd_vel.linear.x = -0.15;
    pubCmdvel.publish(cmd_vel);
  }

  void robotUltra_forward(){
    int start_time, end_time;
    double distance;

    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);

    while(ros::ok())
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
      distance = (end_time - start_time)/29./2.;
      //로봇 이동
      if(distance > 10){ //거리가 10cm 초과이면
        printf("distance: %.2f cm\n", distance);
        this->robotGo(); //로봇 전진
      }
      else{
        this->robotStop(); //10cm 이하이면 로봇 정지
        delayMicroseconds(10);
        break;
      }
    }
  }

  void robotUltra_backward()
  {
    int start_time, end_time;
    double distance;

    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);

    while(ros::ok())
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
      distance = (end_time - start_time)/29./2.;
      //로봇 이동
      if(distance < 15){ //거리가 15cm 미만이면
        printf("distance: %.2f cm\n", distance);
        this->robotBack(); //로봇 후진
      }
      else{
        this->robotStop2();  //15cm 이상이면 로봇 정지
        delayMicroseconds(10);
        break;
      }
    }
  }

  void subUltraSonic(const std_msgs::Int16 subUltraSonic)
  {
    switch(subUltraSonic.data)
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

  //Publisher
  ros::Publisher pubCmdvel;
  ros::Publisher pubArrivalUltraSonicForward;
  ros::Publisher pubArrivalUltraSonicBackward;

  //Subscriber
  ros::Subscriber subCheckUltraSonic;

  //msgs
  geometry_msgs::Twist cmd_vel;
  std_msgs::Int16 fin;
  std_msgs::Int16 fin2;
};

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "UltraSensor");

  //Create an object of class Ultra
  Ultra ultra;

  ROS_INFO("Waiting for robot...");
  wiringPiSetup();

  ros::spin();
  return 0;
}
