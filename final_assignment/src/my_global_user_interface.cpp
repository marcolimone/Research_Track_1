/**
* \file my_global_user_interface.cpp
* \brief Interface to control a robot
* \author Marco Limone
* \version 1.0
* \date 27/02/2022
* \details
*
* Subscribes to: <BR>
* ° /odom
* ° /scan  
* ° /move_base/feedback
*
* Publishes to: <BR>
* ° /move_base/goal
* ° /cmd_vel
* ° move_base/cancel
*
* Description :
*
* we create a user interface that allow the user to control the robot in three different modes
**/


#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "actionlib_msgs/GoalID.h"
#include "move_base_msgs/MoveBaseActionFeedback.h"
#include "mode_srv/Mode.h"
#include "first_mode_srv/Firstmode.h"
#include "controller_srv/Controller.h"
using namespace std;

///<declaration of services
ros::ServiceClient client2;
ros::ServiceClient client1;
ros::ServiceClient client3;

///<declaretion of publishers
ros::Publisher pub;               ///<publish coordinates for mode 1
ros::Publisher pub2;              ///<publish the velocity of the robot
ros::Publisher pub3;              ///< publish the id of the goal to cancel

///<global variables for the mode to use
int mode_1 = 0;
int mode_2 = 0;
int mode_3 = 0;
///<global variables for input by user
int command;
int i;
int comm;
int mode;
int change;


std::string id = "";

/**
* \brief currentStatus callback
* \param msg goal id
* 
* callback to have the goal id and allow its cancellation
**/


void currentStatus(const move_base_msgs::MoveBaseActionFeedback::ConstPtr& msg) {     
if (id != msg->status.goal_id.id) {
        
        id = msg->status.goal_id.id;                                                   ///<put the id in a global variable
    }

}

/**
* \brief clbk_laser callback
* \param msg distances of obstacles
* 
* calback to make the check on the distances and set the behavior of the robot for mode 3
**/


void clbk_laser(const sensor_msgs::LaserScan::ConstPtr& msg){  
                          
    geometry_msgs::Twist vel;
    controller_srv::Controller drive;
    actionlib_msgs::GoalID canc_goal;
    float rright = 30.0;
    float fright = 30.0;
    float front = 30.0;
    float fleft = 30.0;
    float lleft = 30.0;   
    
    ///<search the min value for every sector of the laser scan 
    for (i = 0; i < 143; i++) { //right
        if (msg->ranges[i] < rright)
            rright = msg->ranges[i];
            
    }
    
    for (i = 144; i < 287; i++) { ///<fright
        if (msg->ranges[i] < fright)
            fright = msg->ranges[i];
            
    }
    
    for (i = 288; i < 431; i++) { ///<front
        if (msg->ranges[i] < front)
            front = msg->ranges[i];
            
    }
    
     for (i = 432; i < 575; i++) { ///<fleft
        if (msg->ranges[i] < fleft)
            fleft = msg->ranges[i];
            
    }
    
    for (i = 376; i < 719; i++) { ///<left
        if (msg->ranges[i] < lleft)
            lleft = msg->ranges[i];
            
    }
    ///<drive assistance of mode 3
    if (mode_3 == 1){
        printf("mode3");
        canc_goal.id = id;                    ///<cancel goal
        pub3.publish(canc_goal); 
        if ((rright < 0.5) or (fright < 0.5) or (front < 0.5) or (fleft <0.5) or (lleft < 0.5) ){
           if (front < 1.0 ){
           
           vel.linear.x = -0.6;                                                                        ///<go back if the wall in front of the robot is too near
           vel.angular.z = 0;
           pub2.publish(vel);
           }
           else if (rright < 0.5){                                                                     ///<turn left if the wall on the right is too near
           
           vel.linear.x = 0;
           vel.angular.z = 0.3;
           pub2.publish(vel);
           }
           else if (lleft < 0.5){                                                                      ///<turn right if the wall on the left is too near
          
           vel.linear.x = 0;
           vel.angular.z = -0.3;
           pub2.publish(vel);
           }
        }
        ///<command to drive the robot, same as mode 2
        else {
        
        drive.request.command= 2;
        drive.request.direction_in= 1;
        client3.waitForExistence();
        client3.call(drive);
        command = drive.response.direction_out;
        
        if (command == 1){
          
           vel.linear.x = 0.6;
           vel.angular.z = 0;
           }
        else if (command == 2){
           vel.linear.x = 0;
           vel.angular.z = 0;
           }
        else if (command == 3){
           vel.linear.x = 0;
           vel.angular.z = -0.5; 
           }
        else if (command == 4){
           vel.linear.x = 0;
           vel.angular.z = 0.5;
           }
        else if (command == 5){                   
           mode_3=0;
           mode_1=1;
           } 
        else if (command == 6){                  
           mode_3=0;
           mode_2=1;
           } 
           pub2.publish(vel);     
           }
     }
    return;
    }

/**
* \brief userCallback
* \param msg 
* 
* callback for the user interface and set the behavior of the robot for mode 1,2
**/


void userCallback(const nav_msgs::Odometry::ConstPtr& msg){                                     
 geometry_msgs::Twist my_vel; 
 move_base_msgs::MoveBaseActionGoal coordinates;
 actionlib_msgs::GoalID canc_goal;
 first_mode_srv::Firstmode mode1_coordinates;
 controller_srv::Controller drive;
 mode_srv::Mode change_mode;
 
 float x;
 float y;
 
  ///<setting of the mode using service
   
   change_mode.request.type= 2;
   change_mode.request.in_mode= 5;
   client1.waitForExistence();
   client1.call(change_mode);
   mode_1=change_mode.response.out_mode1;
   mode_2=change_mode.response.out_mode2;
   mode_3=change_mode.response.out_mode3;
   printf(" %d, %d, %d ", mode_1, mode_2, mode_3);
 
 
 
 ///<MODE 1: insert coordinates to allow the robot to reach them
 if (mode_1 == 1){
    printf("mode1");
    ///<Set of a new position 
    mode1_coordinates.request.typereq= 2;
    mode1_coordinates.request.x_in= 1.0;
    mode1_coordinates.request.y_in= 1.0;
    client2.waitForExistence();
    client2.call(mode1_coordinates);
    x = mode1_coordinates.response.x_out;
    y = mode1_coordinates.response.y_out;
    
    printf(" %f, %f", x, y);
    ///<publish the insert position
    coordinates.goal.target_pose.header.frame_id = "map";
    coordinates.goal.target_pose.pose.position.x = x;
    coordinates.goal.target_pose.pose.position.y = y;
    coordinates.goal.target_pose.pose.orientation.w = 1.0;
    pub.publish(coordinates);
    
    
}

  

///<MODE 2: allow the user to controll the robot by keyboard
else if (mode_2 == 1){
        printf("mode2");
        canc_goal.id = id;                    ///<cancel goal
        pub3.publish(canc_goal); 
        drive.request.command= 2;
        drive.request.direction_in= 1;
        client3.waitForExistence();
        client3.call(drive);
        comm = drive.response.direction_out;    
        
        if (comm == 1){
           my_vel.linear.x = 0.6;         
           my_vel.angular.z = 0;
           }
        else if (comm == 2){             
           my_vel.linear.x = 0;
           my_vel.angular.z = 0;
           }
        else if (comm == 3){              
           my_vel.linear.x = 0;
           my_vel.angular.z = -0.5; 
           }
        else if (comm == 4){              
           my_vel.linear.x = 0;
           my_vel.angular.z = 0.5; 
           }
        else if (comm == 5){             
           mode_2=0;
           mode_1=1; 
           }
        else if (comm == 6){              
           mode_2=0;
           mode_3=1; 
           }
        
        pub2.publish(my_vel);
       }     
}

/**
* \brief main
* 
* main function
**/

int main (int argc, char **argv)

{
   ///< Initialize the nodes 
   ros::init(argc, argv, "user_interface");
   ros::NodeHandle n1;
   ros::NodeHandle n4;
   
   
   
   
   
   
   ///< Define the subscribers    
   ros::Subscriber sub1 = n4.subscribe("/odom", 1,userCallback);
   ros::Subscriber sub = n1.subscribe("/scan", 1,clbk_laser);  
   ros::Subscriber sub3 = n4.subscribe("/move_base/feedback", 1000, currentStatus);
     
   ///<Define the publishers  
   pub = n4.advertise<move_base_msgs::MoveBaseActionGoal> ("/move_base/goal", 1) ;
   pub2 = n1.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
   pub3 = n4.advertise<actionlib_msgs::GoalID>("move_base/cancel", 1000);
   
   ///<Define clients
   client1 = n4.serviceClient<mode_srv::Mode>("/mode");
   client2 = n4.serviceClient<first_mode_srv::Firstmode>("/first_mode");
   client3 = n4.serviceClient<controller_srv::Controller>("/controller");
   
   ///<start Multi-threading
   ros::AsyncSpinner spinner(3); 
   spinner.start();
   ros::waitForShutdown();
   
   

}
