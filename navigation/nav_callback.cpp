#include <fstream>
#include <iostream>

#include "moveToGoal.h"
#include <std_msgs/Int16.h>

using namespace std;

bool goalReached = false;

class NavCall
{
public:
  NavCall()
  {
    pubUltraSonic = nh.advertise<std_msgs::Int16>("/ultra", 1);
    pubMotor      = nh.advertise<std_msgs::Int16>("/motor", 1);

    subArrivalUltraSonicForward  = nh.subscribe("/ultra_forward_arrival", 1, &NavCall::CheckArrivalUltraSonicForward, this);
    subArrivalMotor              = nh.subscribe("/motor_arrival", 1, &NavCall::CheckArrivalMotor, this);
    subArrivalUltraSonicBackward = nh.subscribe("/ultra_backward_arrival", 1, &NavCall::ArrivalUltraSonicBackward, this);
  }

  void CheckArrivalUltraSonicForward(const std_msgs::Int16 rcvForwardResult)
  {
    ROS_INFO("Received : %d", rcvForwardResult.data);

    if(rcvForwardResult.data == 2)
    {
      //Read product list from order
      ifstream order("/home/hyun/navigation/src/navigation/src/order.txt");

      int j;
      string list[30];
      int count = 0; int count1 = 0; int count2 = 0;
      int k=0; int n=0; int t=0;

      fill_n(list, 30, "0"); // Initiate list to "0"

      for (j=0;j<30;j++)
      {
        order >> list[j];
        list[j].erase(remove(list[j].begin(),list[j].end(), '['),list[j].end());
        list[j].erase(remove(list[j].begin(),list[j].end(), ']'),list[j].end());
        list[j].erase(remove(list[j].begin(),list[j].end(), ','),list[j].end());
        if(list[j] == "1") count++;
        if(list[j] == "2") count1++;
        if(list[j] == "3") count2++;
      }

      if(count != 0){
        while(k < count){
          First.data = 101;
          pubMotor.publish(First);
          sleep(3);
          k++;
        }
      }

      if(count1 != 0){

        while(n < count1){
          Second.data = 102;
          pubMotor.publish(Second);
          sleep(3);
          n++;
        }
      }

      if(count2 != 0){
        while(t < count2){
          Third.data = 103;
          pubMotor.publish(Third);
          sleep(3);
          t++;
        }
      }
      order.close();
    }
  }

  void CheckArrivalMotor(const std_msgs::Int16 rcvMotorResult)
  {
    ROS_INFO("Received : %d", rcvMotorResult.data);
    if(rcvMotorResult.data == 3)
    {
      Backward.data = 2;
      pubUltraSonic.publish(Backward);
    }
  }

  void ArrivalUltraSonicBackward(const std_msgs::Int16 rcvBackwardResult)
  {
    ROS_INFO("Received : %d", rcvBackwardResult.data);
    if(rcvBackwardResult.data == 4)
    {
      ifstream floors("/home/hyun/navigation/src/navigation/src/floor.txt");
      for(i=0;i<20;i++)
      {
        floors >> number[i];
      }

      ifstream order("/home/hyun/navigation/src/navigation/src/order.txt");
      if(!order.eof())
      {
        for (j=0;j<30;j++)
        {
          order >> shelf[j];
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), '['),shelf[j].end());
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), ']'),shelf[j].end());
          shelf[j].erase(remove(shelf[j].begin(),shelf[j].end(), ','),shelf[j].end());

          if(shelf[j] == "4" || shelf[j] == "5" || shelf[j] == "6") //shelf2
          {
            ROS_INFO("Moving to Shelf2...");

            goalReached = moveToGoal(number[4],number[5],number[6],number[7]);
            if(goalReached)
            {
              ROS_INFO("Shelf2 Reached");
              remove("/home/hyun/navigation/src/navigation/src/socket.txt");
              ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
              fill_n(shelf, 30, "0");
              fout.close();
            }
            sleep(2);

            if(shelf[j] == "7" || shelf[j] == "8" || shelf[j] == "9") //shelf3
            {
              ROS_INFO("Moving to Shelf3...");
              goalReached = moveToGoal(number[8],number[9],number[10],number[11]);
              if(goalReached)
              {
                ROS_INFO("Shelf3 Reached");
              }
            }

            else
            {
              ROS_INFO("Moving to Packing Place");
              goalReached = moveToGoal(number[12],number[13],number[14],number[15]);
              if(goalReached)
              {
                ROS_INFO("Packing Place Reached");
                break;
              }
            }
          }

          else if(shelf[j] == "7" || shelf[j] == "8" || shelf[j] == "9") //shelf3
          {
            ROS_INFO("Moving to Shelf3...");
            goalReached = moveToGoal(number[8],number[9],number[10],number[11]);
            if(goalReached)
            {
              ROS_INFO("Shelf3 Reached");
              fill_n(shelf, 30, "0");
              remove("/home/hyun/navigation/src/navigation/src/socket.txt");
              ofstream fout("/home/hyun/navigation/src/navigation/src/socket.txt", ios::trunc);
              fout.close();
            }
            sleep(2);

            if(shelf[j] == "4" || shelf[j] == "5" || shelf[j] == "6") //shelf2
            {
              ROS_INFO("Moving to Shelf2...");
              goalReached = moveToGoal(number[4],number[5],number[6],number[7]);
              if(goalReached)
              {
                ROS_INFO("Shelf2 Reached");
              }
            }

            else
            {
              ROS_INFO("Moving to Packing Place");
              goalReached = moveToGoal(number[12],number[13],number[14],number[15]);
              if(goalReached)
              {
                ROS_INFO("Packing Place Reached");
                break;
              }
            }
          }
        }
      }
    }
  }

private:
  ros::NodeHandle nh;

  //Publisher
  ros::Publisher pubUltraSonic;
  ros::Publisher pubMotor;

  //Subscriber
  ros::Subscriber subArrivalUltraSonicForward;
  ros::Subscriber subArrivalMotor;
  ros::Subscriber subArrivalUltraSonicBackward;

  std_msgs::Int16 Forward;
  std_msgs::Int16 Backward;
  std_msgs::Int16 First;
  std_msgs::Int16 Second;
  std_msgs::Int16 Third;

  int i, j;
  double number[20];
  string shelf[30];
};


int main(int argc, char** argv){
  //Initiate ROS
  ros::init(argc, argv, "nav_callback");

  ROS_INFO("Navigation_Callback connected");

  //Create an object of class Navcall
  NavCall navCall;

  ros::spin();

  return 0;
}
