#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "std_srvs/Empty.h"
#include "my_car_srv/Velocity_car.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
using namespace std;

ros::Publisher pub;
ros::ServiceClient client2;                             //define custom service
ros::ServiceClient client1;                             //define reset service

void carCallback(const nav_msgs::Odometry::ConstPtr& msg)
{  
   
  
   
   ROS_INFO("Vel subscriber@[%f]",
   msg->twist.twist.linear.x );
   my_car_srv::Velocity_car change_vel;
   geometry_msgs::Twist my_vel;
   std_srvs::Empty srv1;
   
   float vel;
   
   
   int command;
   cin >> command;                                               //get the command passed by the user with the keyboard
 
   printf("Send 0 to reset, 1 to accelerate, 2 to decellerate \n");
   if (command == 0)                                             //command 0 call the reset service
   {
   client1.waitForExistence();
   client1.call(srv1);
   }
   else if(command == 1)                                         //command 1 pass the command 1 to the custom service
   {
     change_vel.request.velocity= 1;
     client2.waitForExistence();
     client2.call(change_vel);
    
   }
   else if(command == 2)                                         //command 2 pass the command 2 to the custom service 
   {
    
     change_vel.request.velocity= 2;
     client2.waitForExistence();
     client2.call(change_vel);
   }
   
 
}



int main (int argc, char **argv)

{
   // Initialize the node, setup the NodeHandle for handling the communication with the ROS
   //system
   ros::init(argc, argv, "vel_subscriber");
   ros::NodeHandle n2;
   ros::Subscriber sub = n2.subscribe("/odom", 1,carCallback);
   client1 = n2.serviceClient<std_srvs::Empty>("/reset_positions");               //initialize reset service
   client2 = n2.serviceClient<my_car_srv::Velocity_car>("/velocity_car");         //initialize custom service
   pub = n2.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
   
   ros::spin();
   return 0;
}
