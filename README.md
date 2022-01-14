# Assignment_3 Marco Limone

In the third assignment I have a robot located on a map that it doesn't know about. Thanks to the lasers the robot is supplied with, it is able to see obstacles and build the map in which it acts as it explores it.
The assignment consist in the development of a software architecture for the control of a mobile robot in the environment. The software will rely on the move_base
and gmapping packages for localizing the robot and plan the motion. The user can choose between the following three control modes for the robot:
- 1) autonomously reach a x,y coordinate inserted by the user
- 2) let the user drive the robot with the keyboard
- 3) let the user drive the robot assisting them to avoid collisions                                                                   

To achieve this goal, a node has been created which acts as an interface for the user and at the same time manages the movements of the robot. In this node I initialize two subscriber. The first subscriber is subscribed to the /odom topic and its callback contains the part of the user interface of the three mode. For the first mode there is the request of the coordinates that the robot must achieve and than them are sent to the topic move_base/goal. For the second mode the user have to sent a command depending of what he want that the robot will do. Depending on the command received, are set linear and angular velocity publishing them by the topic /cmd_vel. For the third is different because in the first callback there is only the acquisition of the command. In the second callback, which subscriber is subscribed to the laser scan topic, there is the check of the distance of the walls and there is the logic of the assistent drive that let to avoid collisions. 
To use the robot in the simulation environment, just launch the "final_assignment.launch" launch file which imports the "move_base" and "gmapping" launch files into it and also the node that manages the robot(it is located in the src folder). The command to run is the following : 
roslaunch final_assignment final_assignment.launch

PSEUDO-CODE

include all type og messages we need
declaretion of publishers
initializate global variables 


void currentStatus(:MoveBaseActionFeedback msg)  {      
if (variable id != from the actual goal ID) {
        
        set id equal to actual ID;                                                   
    }

}

void clbk_laser(LaserScan msg){                        
    geometry_msgs::Twist vel;
    
    float rright = 30.0;
    float fright = 30.0;
    float front = 30.0;
    float fleft = 30.0;
    float lleft = 30.0;   
    
    search the min value for every sector of the laser scan 
    
    drive assistance of mode 3
    if (mode_3 is active){
        if (one of the min value is minor than 0,5){
           if (front < 1.0 ){
           robot does not go on;
           }
           else if (rright < 0.5){                                                                     
           turn in order to not crash
           }
           else if (lleft < 0.5){                                                                     
           turn in order to not crash
           }
        }
        command to drive the robot, same as mode 2
        else {
        
        if (command == 1){
           set linear and angular velocity to go on;
           }
        else if (command == 2){
           set linear and angular velocity to stop;
           }
        else if (command == 3){
           set linear and angular velocity to turn right;
           }
        else if (command == 4){
           set linear and angular velocity to turn left;
           }
        else if (command == 5){                   
           set current mode variable to 0 and mode 1 variable to 1 to change mode;
           } 
        else if (command == 6){                   
           set current mode variable to 0 and mode 3 variable to 1 to change mode;
           } 
           publish velocity;     
           }
     }
    return;
    }


void userCallback(Odometry msg)

{        

 geometry_msgs::Twist my_vel; 
 move_base_msgs::MoveBaseActionGoal coordinates;
 actionlib_msgs::GoalID canc_goal;
 
 float x;
 float y;
 
 
 //MODE 1 
 
 if (mode_1 == 1){
    
    Ask the user for a new position 
    cin >> x;
    cin >> y;
    publish the insert position
    Ask the user if he want to chage mode or cancel the goal 
    cin >> change;
    if (change == 2 ) {
       cancel goal; 
    }        
    else if (change == 3){ 
       cancel goal and set mode 2 variable to 1
    }      
    else if (change == 4){
       cancel goal and set mode 3 variable to 1
    }   
    
}

//MODE 3 

if (mode_3 == 1) {

    Ask the user what the robot have to do or if he want to change mode 
    cin >> command;
 }    

//MODE 2 

else if (mode_2 == 1){

        Ask the user what the robot have to do or if he want to change mode 
        cin >> comm;
        if (comm == 1){
           set linear and angular velocity
           }
        else if (comm == 2){             
           set linear and angular velocity
           }
        else if (comm == 3){              
           set linear and angular velocity
           }
        else if (comm == 4){              
           set linear and angular velocity 
           }
        else if (comm == 5){              
           set current mode variable to 0 and mode 1 variable to 1 to change mode
           }
        else if (comm == 6){              
           set current mode variable to 0 and mode 3 variable to 1 to change mode
           }
        publish velocity;
       }     
}

int main (int argc, char **argv):

{

 Initialize the nodes 
 request of the mode to use
 printf()
 take the keyboard command  
 setting of the mode using global variables
 
 
 if (mode == 1){
 
        set the global variable for mode 1 to 1;
       }
 else if (mode == 2){ 
 
       set the global variable for mode 2 to 1;
       }
 else if (mode == 3){
 
       set the global variable for mode 3 to 1;
       }
   
  Define the subscribers with their callback    
  
     
  Define the publishers  
   
   
  start Multi-threading
  ros::AsyncSpinner spinner(3); 
  spinner.start();
  ros::waitForShutdown();
   
   

}
