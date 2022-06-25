//Header
#include <fstream>
#include <iostream>

#include "moveToGoal.h"
#include <std_msgs/Int16.h>

using namespace std;

bool goalReached = false;

class Navsock
{
public:
  Navsock()
  {
    pubUltraSonic = nh.advertise<std_msgs::Int16>("/ultra", 1);

    Move();
  }

  void Move()
  {
    while(ros::ok()){
      //Read coordinate
      ifstream floors("/home/hyun/navigation/src/navigation/src/floor.txt");
      for(i=0; i<20; i++){
        floors >> number[i];
      }
      //Read msg from app
      ifstream socket("/home/hyun/navigation/src/navigation/src/socket.txt");
      if(!socket.eof())
      {
        for (j=0;j<30;j++)
        {
          socket >> shelf[j];
          //Delete '[' , ',' , ']' from list
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), '['),shelf[j].end());
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), ']'),shelf[j].end());
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), ','),shelf[j].end());

          //Shelf1
          if(shelf[j] == "1" || shelf[j] == "2" || shelf[j] == "3")
          {
            ROS_INFO("Moving to Shelf...");
            goalReached = moveToGoal(number[0],number[1],number[2],number[3]);
            if(goalReached)
            {
              ROS_INFO("Shelf Reached");
              fill_n(shelf,30,'0');
              Forward.data = 1;
              pubUltraSonic.publish(Forward);
              remove("/home/hyun/navigation/src/navigation/src/socket.txt");
              ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
              fout.close();
              break;
            }
          }

          else if(shelf[j] == "10")
          {
            ROS_INFO("Moving to Station...");
            goalReached = moveToGoal(number[16],number[17],number[18],number[19]);
            if(goalReached)
            {
              ROS_INFO("Station Reached");
              fill_n(shelf,30,'0');
              remove("/home/hyun/navigation/src/navigation/src/socket.txt");
              ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
              fout.close();
              break;
            }
          }
        }
      }
      floors.close();
      socket.close();
    }
  }

private:
  ros::NodeHandle nh;

  //Publisher
  ros::Publisher pubUltraSonic;

  //Variable
  std_msgs::Int16 Forward;
  int i,j;
  double number[20];
  string shelf[30];
};


int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "nav_socket");

  ROS_INFO("Navigation_Socket connected");

  //Initiate Socket File
  ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
  fout.close();

  //Create an object of class Navsock
  Navsock navsock;

  ros::spin();

  return 0;
}
