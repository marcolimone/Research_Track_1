#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "my_car_srv/Velocity_car.h"

ros::ServiceClient client1;     /*define custom service*/
ros::Publisher pub;             /*define publisher*/
float vel=0.0;                  /*initialize global variable*/
int count;



void velCallback (const nav_msgs::Odometry::ConstPtr& msg)
{  
   
  
   
   ROS_INFO("Vel subscriber@[%f]",
   msg->twist.twist.linear.x );
   my_car_srv::Velocity_car my_vel;
   
   my_vel.request.velocity= 3;                    //request to the service
   client1.waitForExistence();
   client1.call(my_vel);                          //call the service
   vel=my_vel.response.mult;                      //set the global variable vel equal to the sevice response
   
 }
 
void carCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{  
  
   
   
   ROS_INFO("Car subscriber@[%f, %f, %f]",
   msg->ranges[100],msg->ranges[360], msg->ranges[619] );   
   
   
   geometry_msgs::Twist my_vel;                        //define a variable for our publisher
   if(msg->ranges[360]<0.8)                             //check on front distance
      {
        if(msg->ranges[100] < msg->ranges[619])        //check on lateral distances to understand which side to turn to
        {
           count=1;
           my_vel.linear.x = 0.0;                      
           my_vel.angular.z = 0.5;                     //set angula.z to turn right
        }
        else
        {
         count=1;
         my_vel.linear.x = 0.0;
         my_vel.angular.z = -0.5;                      //set angula.z to turn left
        }
      }
   else if( msg->ranges[100] < 0.55)                   //check on left distance
     {  
       count=1;
       my_vel.linear.x = 0.0;
       my_vel.angular.z = 0.5;   
     }
   
   else if(msg->ranges[619] < 0.55)                   //check on right distance
     {
       count=1;
       my_vel.linear.x = 0.0;
       my_vel.angular.z = -0.5;   
     }
   
   else                                               //if all previous check fail, is setted the global value vel
     {
        my_vel.linear.x = vel;
        count=0;
     }
   pub.publish(my_vel);                              //publish my_vel to give the robot a linear or angular velocity
}



int main (int argc, char **argv)

{
   // Initialize the node, setup the NodeHandle for handling the communication with the ROS
  //system
   ros::init(argc, argv, "car_subscriber");
   ros::NodeHandle n1;
   ros::NodeHandle n4;
   ros::Subscriber sub1 = n4.subscribe("/odom", 1,velCallback);              //define and initialize subscriber to /odom and call of velCallback
   client1 = n4.serviceClient<my_car_srv::Velocity_car>("/velocity_car");     //initialize custom service 
   // Define the subscriber to
   ros::Subscriber sub = n1.subscribe("/base_scan", 1,carCallback);          //define and initialize subscriber to /base_scan and call of carCallback
   pub = n1.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);                 //initialize publisher /cmd_vel
   ros::spin();
   return 0;
}
