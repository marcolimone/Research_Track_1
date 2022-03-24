#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "mode_srv/Mode.h"
#include "first_mode_srv/Firstmode.h"
#include "controller_srv/Controller.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
using namespace std;

//ros::Publisher pub;
ros::ServiceClient client2;
ros::ServiceClient client1;
ros::ServiceClient client3;
int mode1=0;
int mode2=0;
int mode3=0;
int comm;
int change;
int flag = 1;
int command;

void userClk (const nav_msgs::Odometry::ConstPtr& msg)
{  
   
   mode_srv::Mode change_mode;
   first_mode_srv::Firstmode mode1_coordinates;
   controller_srv::Controller drive;
   float x;
   float y;
   
  if (flag == 1)
  {
   printf("Choose an action:\n"              
            "1 - Mode 1\n"
            "2 - Mode 2\n"
            "3 - Mode 3\n");
   
   int command;
   cin >> command;
   
   if (command == 1)
   {
   change_mode.request.type= 1;
   change_mode.request.in_mode= 1;
   client1.waitForExistence();
   client1.call(change_mode);
   mode1=1;
   mode2=0;
   mode3=0;
   }
   else if(command == 2)
   {
     change_mode.request.type= 1;
     change_mode.request.in_mode= 2;
     client1.waitForExistence();
     client1.call(change_mode);
     mode1=0;
     mode2=1;
     mode3=0;
   }
   
   else if(command == 3)
   {
     change_mode.request.type= 1;
     change_mode.request.in_mode= 3;
     client1.waitForExistence();
     client1.call(change_mode);
     mode1=0;
     mode2=0;
     mode3=1;
   }
   flag = 0;
   }
   
   if(mode1 == 1)
   {
     printf("Insert a new position \n");
     printf("New x position: \n");
     cin >> x;
     printf("New y position: \n");
     cin >> y;
     
     mode1_coordinates.request.typereq= 1;
     mode1_coordinates.request.x_in= x;
     mode1_coordinates.request.y_in= y;
     client2.waitForExistence();
     client2.call(mode1_coordinates);
     
     printf("Choose an action:\n"              
            "1 - go on with this mode\n"
            "2 - change mode 2\n");
     cin >> command;
     if (command == 2)
     {
       flag = 1;
     }       
     
   }
   
  
   
   if(mode2 == 1)
   {
     printf("\nCommands:\n"
            "1 - Go straight\n"
            "2 - Stop\n"
            "3 - Turn right\n"
            "4 - Turn left\n"
            "5 - Change mode\n"
            
            );
            
     cin >> comm;
     if (comm == 5)
     {
       flag = 1;
     }  
     else
     {
     drive.request.command= 1;
     drive.request.direction_in= comm;
     client3.waitForExistence();
     client3.call(drive);
     }
   }
   
   if(mode3 == 1)
   {
     printf("\nCommands:\n"
            "1 - Go straight\n"
            "2 - Stop\n"
            "3 - Turn right\n"
            "4 - Turn left\n"
            "5 - Change mode\n"
            );
     cin >> comm;
     
     if (comm == 5)
     {
       flag = 1;
     }  
     else
     {
     drive.request.command= 1;
     drive.request.direction_in= comm;
     client3.waitForExistence();
     client3.call(drive);
     }
   }
   
 
}



int main (int argc, char **argv)

{
   // Initialize the node, setup the NodeHandle for handling the communication with the ROS
   //system
   ros::init(argc, argv, "user_subscriber");
   ros::NodeHandle n2;
   ros::Subscriber sub = n2.subscribe("/odom", 1, userClk);
   //mode_srv::Mode change_mode;
   
   
   
   
   client1 = n2.serviceClient<mode_srv::Mode>("/mode");
   client2 = n2.serviceClient<first_mode_srv::Firstmode>("/first_mode");
   client3 = n2.serviceClient<controller_srv::Controller>("/controller");
   //pub = n2.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
   
   ros::spin();
   return 0;
}
