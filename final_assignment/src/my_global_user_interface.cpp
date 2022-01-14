
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include "actionlib_msgs/GoalID.h"
#include "move_base_msgs/MoveBaseActionFeedback.h"
using namespace std;

//declaretion of publishers
ros::Publisher pub;
ros::Publisher pub2;
ros::Publisher pub3;

//global variables for the mode to use
int mode_1 = 0;
int mode_2 = 0;
int mode_3 = 0;
//global variables for input by user
int command;
int i;
int comm;
int mode;
int change;

std::string id = "";

void currentStatus(const move_base_msgs::MoveBaseActionFeedback::ConstPtr& msg) {     //callback to have the goal id and allow its cancellation 
if (id != msg->status.goal_id.id) {
        
        id = msg->status.goal_id.id;                                                   //put the id in a global variable
    }

}

void clbk_laser(const sensor_msgs::LaserScan::ConstPtr& msg){                        //calback to make the check on the distances and set the behavior of the robot for mode 3
    geometry_msgs::Twist vel;
    
    float rright = 30.0;
    float fright = 30.0;
    float front = 30.0;
    float fleft = 30.0;
    float lleft = 30.0;   
    
    //search the min value for every sector of the laser scan 
    for (i = 0; i < 143; i++) { //right
        if (msg->ranges[i] < rright)
            rright = msg->ranges[i];
            
    }
    
    for (i = 144; i < 287; i++) { //fright
        if (msg->ranges[i] < fright)
            fright = msg->ranges[i];
            
    }
    
    for (i = 288; i < 431; i++) { //front
        if (msg->ranges[i] < front)
            front = msg->ranges[i];
            
    }
    
     for (i = 432; i < 575; i++) { //fleft
        if (msg->ranges[i] < fleft)
            fleft = msg->ranges[i];
            
    }
    
    for (i = 376; i < 719; i++) { //left
        if (msg->ranges[i] < lleft)
            lleft = msg->ranges[i];
            
    }
    //drive assistance of mode 3
    if (mode_3 == 1){
        if ((rright < 0.5) or (fright < 0.5) or (front < 0.5) or (fleft <0.5) or (lleft < 0.5) ){
           if (front < 1.0 ){
           
           vel.linear.x = -0.6;                                                                        //go back if the wall in front of the robot is too near
           vel.angular.z = 0;
           pub2.publish(vel);
           }
           else if (rright < 0.5){                                                                     //turn left if the wall on the right is too near
           
           vel.linear.x = 0;
           vel.angular.z = 0.3;
           pub2.publish(vel);
           }
           else if (lleft < 0.5){                                                                      //turn right if the wall on the left is too near
          
           vel.linear.x = 0;
           vel.angular.z = -0.3;
           pub2.publish(vel);
           }
        }
        //command to drive the robot, same as mode 2
        else {
        
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
        else if (command == 5){                   //pass to mode 1
           mode_3=0;
           mode_1=1;
           } 
        else if (command == 6){                   //pass to mode 2
           mode_3=0;
           mode_2=1;
           } 
           pub2.publish(vel);     
           }
     }
    return;
    }


void userCallback(const nav_msgs::Odometry::ConstPtr& msg){                                     //callback for the user interface and set the behavior of the robot for mode 1,2
 geometry_msgs::Twist my_vel; 
 move_base_msgs::MoveBaseActionGoal coordinates;
 actionlib_msgs::GoalID canc_goal;
 
 float x;
 float y;
 
 
 //MODE 1
 if (mode_1 == 1){
    //Ask the user for a new position 
    printf("Insert a new position \n");
    printf("New x position: \n");
    cin >> x;
    printf("New y position: \n");
    cin >> y;
    //publish the insert position
    coordinates.goal.target_pose.header.frame_id = "map";
    coordinates.goal.target_pose.pose.position.x = x;
    coordinates.goal.target_pose.pose.position.y = y;
    coordinates.goal.target_pose.pose.orientation.w = 1.0;
    pub.publish(coordinates);
    //Ask the user if he want to chage mode or cancel the goal 
    printf("Thanks! Let's reach the next position \n"
            "1 - Go on with this mode\n"
            "2 - Cancel goal\n"
            "3 - Change mode: Mode 2\n"
            "4 - Change mode: Mode 3\n");
    cin >> change;
    if (change == 2 ) {
            canc_goal.id = id;                    //cancel goal
            pub3.publish(canc_goal); 
    }        
    else if (change == 3){ 
       canc_goal.id = id;                        //cancel goal and pass to mode 2
       pub3.publish(canc_goal);                          
       mode_1=0;
       mode_2=1;
    }      
    else if (change == 4){
       canc_goal.id = id;                    //cancel goal and pass to mode 3
       pub3.publish(canc_goal);                              
       mode_1=0;
       mode_3=1;
    }   
    
}

//MODE 3
if (mode_3 == 1) { 
    //Ask the user what the robot have to do or if he want to change mode 
    printf("\nCommands:\n"
            "1 - Go on\n"
            "2 - Stop\n"
            "3 - Turn right\n"
            "4 - Turn left\n"
            "-----------------------------\n"
            "5 - Pass to mode 1\n"
            "6 - Pass to mode 2\n"
            );
     cin >> command;
    
}    

//MODE 2
else if (mode_2 == 1){
         //Ask the user what the robot have to do or if he want to change mode 
         printf("\nCommands:\n"
            "1 - Go straight\n"
            "2 - Stop\n"
            "3 - Turn right\n"
            "4 - Turn left\n"
            "-----------------------------\n"
            "5 - Pass to mode 1\n"
            "6 - Pass to mode 3\n"
            );
            
        cin >> comm;
        if (comm == 1){
           my_vel.linear.x = 0.6;         //go straight
           my_vel.angular.z = 0;
           }
        else if (comm == 2){              //stop
           my_vel.linear.x = 0;
           my_vel.angular.z = 0;
           }
        else if (comm == 3){              //turn right
           my_vel.linear.x = 0;
           my_vel.angular.z = -0.5; 
           }
        else if (comm == 4){              // turn left
           my_vel.linear.x = 0;
           my_vel.angular.z = 0.5; 
           }
        else if (comm == 5){              //pass to mode 1
           mode_2=0;
           mode_1=1; 
           }
        else if (comm == 6){              //pass to mode 3
           mode_2=0;
           mode_3=1; 
           }
        
        pub2.publish(my_vel);
       }     
}
int main (int argc, char **argv)

{
   // Initialize the nodes 
   ros::init(argc, argv, "user_interface");
   ros::NodeHandle n1;
   ros::NodeHandle n4;
   
   //request of the mode to use
   printf("Choose an action:\n"              
            "1 - Mode 1\n"
            "2 - Mode 2\n"
            "3 - Mode 3\n");
   cin >> mode;
   //setting of the mode using global variables
   if (mode == 1){
       mode_1 = 1;
       }
   else if (mode == 2){   
       mode_2 = 1;
       }
   else if (mode == 3){
       mode_3 = 1;
       }
   
   //// Define the subscribers    
   ros::Subscriber sub1 = n4.subscribe("/odom", 1,userCallback);
   ros::Subscriber sub = n1.subscribe("/scan", 1,clbk_laser);  
   ros::Subscriber sub3 = n4.subscribe("/move_base/feedback", 1000, currentStatus);
     
   //Define the publishers  
   pub = n4.advertise<move_base_msgs::MoveBaseActionGoal> ("/move_base/goal", 1) ;
   pub2 = n1.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
   pub3 = n4.advertise<actionlib_msgs::GoalID>("move_base/cancel", 1000);
   
   //start Multi-threading
   ros::AsyncSpinner spinner(3); 
   spinner.start();
   ros::waitForShutdown();
   
   

}
