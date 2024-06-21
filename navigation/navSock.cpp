#include <fstream>
#include <iostream>
#include <std_msgs/Int16.h>
#include <ros/ros.h>
#include <vector>
#include <algorithm>
#include "moveToGoal.h"

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
        while (ros::ok())
        {
            if (!readNumberFile("floor.txt"))
            {
                ROS_ERROR("Failed to read floor file");
                ros::Duration(1).sleep();
                continue;
            }

            if (!readOrderFile("socket.txt"))
            {
                ROS_ERROR("Failed to read socket file");
                ros::Duration(1).sleep();
                continue;
            }

            processOrders();

            ros::Duration(1).sleep();
        }
    }

private:
    ros::NodeHandle nh;
    ros::Publisher pubUltraSonic;
    std_msgs::Int16 forward;
    vector<double> number(20);
    vector<string> shelf(30);

    bool readNumberFile(const string &filePath)
    {
        ifstream floors(filePath);
        if (!floors.is_open())
        {
            return false;
        }

        for (int i = 0; i < 20; i++)
        {
            if (!(floors >> number[i]))
            {
                return false;
            }
        }
        floors.close();
        return true;
    }

    bool readOrderFile(const string &filePath)
    {
        ifstream socket(filePath);
        if (!socket.is_open())
        {
            return false;
        }

        for (int j = 0; j < 30; j++)
        {
            if (!(socket >> shelf[j]))
                return false;
            shelf[j].erase(remove(shelf[j].begin(), shelf[j].end(), '['), shelf[j].end());
            shelf[j].erase(remove(shelf[j].begin(), shelf[j].end(), ']'), shelf[j].end());
            shelf[j].erase(remove(shelf[j].begin(), shelf[j].end(), ','), shelf[j].end());
        }
        socket.close();
        return true;
    }

    void processOrders()
    {
        for (const auto &s : shelf)
        {
            if (s == "1" || s == "2" || s == "3")
            {
                ROS_INFO("Moving to Shelf...");
                goalReached = moveToGoal(number[0], number[1], number[2], number[3]);
                if (goalReached)
                {
                    ROS_INFO("Shelf Reached");
                    clearOrderFile();
                    forward.data = 1;
                    pubUltraSonic.publish(forward);
                    break;
                }
            }
            else if (s == "10")
            {
                ROS_INFO("Moving to Station...");
                goalReached = moveToGoal(number[16], number[17], number[18], number[19]);
                if (goalReached)
                {
                    ROS_INFO("Station Reached");
                    clearOrderFile();
                    break;
                }
            }
        }
    }

    void clearOrderFile()
    {
        fill(shelf.begin(), shelf.end(), "0");
        ofstream fout("socket.txt", ios::trunc);
        fout.close();
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "nav_socket");

    ofstream fout("socket.txt", ios::trunc);
    fout.close();

    Navsock navsock;

    ros::spin();

    return 0;
}
