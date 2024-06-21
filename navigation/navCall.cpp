#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ros/ros.h>
#include <std_msgs/Int16.h>

#include "moveToGoal.h"

using namespace std;

bool goalReached = false;

class NavCall
{
public:
    NavCall()
    {
        pubMotor = nh.advertise<std_msgs::Int16>("/motor", 1);
        pubUltraSonic = nh.advertise<std_msgs::Int16>("/ultra", 1);

        subArrivalMotor = nh.subscribe("/motor_arrival", 1, &NavCall::CheckArrivalMotor, this);
        subArrivalUltraSonicBackward = nh.subscribe("/ultra_backward_arrival", 1, &NavCall::ArrivalUltraSonicBackward, this);
        subArrivalUltraSonicForward = nh.subscribe("/ultra_forward_arrival", 1, &NavCall::CheckArrivalUltraSonicForward, this);
    }

    void CheckArrivalUltraSonicForward(const std_msgs::Int16 &rcvForwardResult)
    {
        ROS_INFO("Received : %d", rcvForwardResult.data);

        if (rcvForwardResult.data == 2)
        {
            vector<string> list(30);
            vector<int> count(3);
            readOrderFile("order.txt", list, count);

            if (count[0] != 0)
                processOrder(count[0], 101, first);
            if (count[1] != 0)
                processOrder(count[1], 102, second);
            if (count[2] != 0)
                processOrder(count[2], 103, third);
        }
    }

    void CheckArrivalMotor(const std_msgs::Int16 &rcvMotorResult)
    {
        ROS_INFO("Received : %d", rcvMotorResult.data);

        if (rcvMotorResult.data == 3)
        {
            backward.data = 2;
            pubUltraSonic.publish(backward);
        }
    }

    void ArrivalUltraSonicBackward(const std_msgs::Int16 &rcvBackwardResult)
    {
        ROS_INFO("Received : %d", rcvBackwardResult.data);

        if (rcvBackwardResult.data == 4)
        {
            vector<double> number(20);
            vector<string> shelf(30);

            readNumberFile("floor.txt", number);
            readOrderFile("order.txt", shelf);

            processShelves(number, shelf);
        }
    }

private:
    ros::NodeHandle nh;

    ros::Publisher pubMotor;
    ros::Publisher pubUltraSonic;

    ros::Subscriber subArrivalMotor;
    ros::Subscriber subArrivalUltraSonicForward;
    ros::Subscriber subArrivalUltraSonicBackward;

    std_msgs::Int16 forward;
    std_msgs::Int16 backward;
    std_msgs::Int16 first;
    std_msgs::Int16 second;
    std_msgs::Int16 third;

    void readOrderFile(const string &filePath, vector<string> &list, vector<int> &count)
    {
        ifstream order(filePath);
        if (!order.is_open())
        {
            ROS_ERROR("Failed to open order file: %s", filePath.c_str());
            return;
        }

        for (int i = 0; i < 30; i++)
        {
            if (!(order >> list[i]))
                break;
            list[i].erase(remove(list[i].begin(), list[i].end(), '['), list[i].end());
            list[i].erase(remove(list[i].begin(), list[i].end(), ']'), list[i].end());
            list[i].erase(remove(list[i].begin(), list[i].end(), ','), list[i].end());

            if (list[i] == "1")
                count[0]++;
            else if (list[i] == "2")
                count[1]++;
            else if (list[i] == "3")
                count[2]++;
        }

        order.close();
    }

    void processOrder(int count, int data, std_msgs::Int16 &msg)
    {
        for (int i = 0; i < count; i++)
        {
            msg.data = data;
            pubMotor.publish(msg);
            ros::Duration(3).sleep();
        }
    }

    void readNumberFile(const string &filePath, vector<double> &number)
    {
        ifstream floors(filePath);
        if (!floors.is_open())
        {
            ROS_ERROR("Failed to open floor file: %s", filePath.c_str());
            return;
        }

        for (int i = 0; i < 20; i++)
        {
            if (!(floors >> number[i]))
                break;
        }

        floors.close();
    }

    void processShelves(const vector<double> &number, const vector<string> &shelf)
    {
        for (const auto &s : shelf)
        {
            if (s == "4" || s == "5" || s == "6")
            {
                ROS_INFO("Moving to Shelf2...");
                goalReached = moveToGoal(number[4], number[5], number[6], number[7]);

                if (goalReached)
                {
                    ROS_INFO("Shelf2 Reached");
                    clearOrderFile();
                }
                ros::Duration(2).sleep();
            }
            else if (s == "7" || s == "8" || s == "9")
            {
                ROS_INFO("Moving to Shelf3...");
                goalReached = moveToGoal(number[8], number[9], number[10], number[11]);

                if (goalReached)
                {
                    ROS_INFO("Shelf3 Reached");
                    clearOrderFile();
                }
                ros::Duration(2).sleep();
            }
            else
            {
                ROS_INFO("Moving to Packing Place");
                goalReached = moveToGoal(number[12], number[13], number[14], number[15]);

                if (goalReached)
                {
                    ROS_INFO("Packing Place Reached");
                    break;
                }
            }
        }
    }

    void clearOrderFile()
    {
        remove("socket.txt");
        ofstream fout("socket.txt", ios::trunc);
        fout.close();
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "nav_callback");

    NavCall navCall;

    ros::spin();

    return 0;
}
