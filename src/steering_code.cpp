// GOAL OF CPP FILE: The node subscribes and publishes to stuff in the robot0 namespace.
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h" 
#include "sensor_msgs/LaserScan.h"
#include <sstream> 
#include <stdlib.h>
#include <string> 

//Defining needed variables
    float lin_vel = 0;
    float ang_vel = 0;
    std::vector<float> lidar_data;

void LidarCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
    lidar_data = msg-> ranges;
}

void VelCallback(const geometry_msgs::Twist::ConstPtr& msg){
    lin_vel = msg->linear.x;
    ang_vel = msg->angular.z;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "lab2_pkg_node"); 
    ros::NodeHandle handle;

    //Copied from the assignment sheet
    int opt;
    char* topic_name;
    while ((opt = getopt(argc, (argv), "n:")) != -1){
        switch(opt){
            case 'n':
                topic_name = optarg;
                break;
            default:
                printf("The -%c is not a recognized parameter.\n", opt);
                break;
        }
    }
 //End of the copy from the assignment sheet

    ros::Subscriber vel_sub = handle.subscribe("des_vel", 1, VelCallback);
    ros::Subscriber las_sub = handle.subscribe("laser_1", 1, LidarCallback);
    ros::Publisher vel_pub = handle.advertise<geometry_msgs::Twist>("cmd_vel", 1); 
    ros::Rate loop_rate(10); //the loop rate is 10Hz

//Detecting the wall and stopping before impact or robot gets stuck
    while (ros::ok()){ 
        geometry_msgs::Twist message;
        message.linear.x = lin_vel;
        message.angular.z = ang_vel;
        if(lin_vel > 0 && lidar_data [270/2] < 2){
            message.linear.x = 0;
        }
        vel_pub.publish(message);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}