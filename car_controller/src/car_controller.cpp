#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "my_car_srv/Velocity_car.h"

ros::ServiceClient client1;
ros::Publisher pub;
float vel=0.0;
int count;



void velCallback (const nav_msgs::Odometry::ConstPtr& msg)
{  
   
  
   
   ROS_INFO("Vel subscriber@[%f]",
   msg->twist.twist.linear.x );
   my_car_srv::Velocity_car my_vel;
   
   my_vel.request.velocity= 3;
   client1.waitForExistence();
   client1.call(my_vel);
   vel=my_vel.response.mult;
   
 }
 
void carCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{  
  
   
   
   ROS_INFO("Car subscriber@[%f, %f, %f]",
   msg->ranges[100],msg->ranges[360], msg->ranges[619] );
   
   
   geometry_msgs::Twist my_vel;
   if(msg->ranges[360]<0.8)
      {
        if(msg->ranges[100] < msg->ranges[619])
        {
           count=1;
           my_vel.linear.x = 0.0;
           my_vel.angular.z = 0.5;
        }
        else
        {
         count=1;
         my_vel.linear.x = 0.0;
         my_vel.angular.z = -0.5;
        }
      }
   else if( msg->ranges[100] < 0.55)
     {  
       count=1;
       my_vel.linear.x = 0.0;
       my_vel.angular.z = 0.5;   
     }
   
   else if(msg->ranges[619] < 0.55)
     {
       count=1;
       my_vel.linear.x = 0.0;
       my_vel.angular.z = -0.5;   
     }
   
   else 
     {
        my_vel.linear.x = vel;
        count=0;
     }
   pub.publish(my_vel);
}



int main (int argc, char **argv)

{
   // Initialize the node, setup the NodeHandle for handling the communication with the ROS
   //system
   ros::init(argc, argv, "car_subscriber");
   ros::NodeHandle n1;
   ros::NodeHandle n4;
   ros::Subscriber sub1 = n4.subscribe("/odom", 1,velCallback);
   client1 = n4.serviceClient<my_car_srv::Velocity_car>("/velocity_car");
   // Define the subscriber to
   ros::Subscriber sub = n1.subscribe("/base_scan", 1,carCallback);
   pub = n1.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
   ros::spin();
   return 0;
}
