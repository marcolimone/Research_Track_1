# Assignment_2 Marco Limone
In this second assignment I have a robot which should move autonomously inside a circuit and I have to control it with ROS, in a special way I need to have a node that control the movement of the robot in the circuit, and another node that allow the user to increase or decrease and to reset the position of the robot. 
In my code I use two node called "carcontroller_node" and "Velcontroller_node", and a custom service called "server_vel_node".These three nodes allow me respectively to manage the laser scans of the robot so as not to crash (carcontroller_node), to manage the user inputs for acceleration, deceleration and reset of the robot (Velcontroller_node) and the server (server_vel_node) to manage the new calculated speed and make it available for all nodes, in particular for the carcontroller_node.
To use the code the user have to launch the simulator enviroment, than launch the server with the command "rosrun my_car_srv server_vel_node", then he have to launch the controller node with "rosrun car_controller carcontroller_node" and in the end it launch "rosrun vel_controller Velcontroller_node".

carcontrollre_node

This node executes the instructions present in the car_controller.cpp file which in turn is located in the src folder of the pacakge car_controller of my workspace. In the code I use a publisher who publishes the speed values that my robot must have and which are managed in such a way as not to make the robot crash into obstacles. I use also two subscriber: 
the first one gives me the laser scan values in order to detect obstacles and allow the robot to avoid them, in a special way in his callback there is all the logic for the right movement of the robot and for the publication of the correct speed value that the robot must keep;
the second one allows us to have data on the speed at which the robot is traveling, in particular its callback allows us to call up the server of our custom service and set the desired speed.

Velcontroller_node

This node executes the instructions present in the Vel_controller.cpp file which in turn is located in the src folder of the package vel_controller of my workspace. In the code I use a subscriber to /odom topic and in its callback I acquire the user command from the keyboard, and depending on the command I call two services, one for reset the robot position and one for the acceleration or deceleration of the robot.

server_vel_node

This node is entirely organized in the my_car_srv package which contains the srv folder, where is located the Velocity_car.srv file  which contains the structure of the message that the servic is going to manage, and the src folder where is located the file car_server.cpp which contains the istruction that performs the service.


PSEUDO-CODE car_controlle.cpp

void callback  di /odom(define /odom message)

{  ROS_INFO(message to screen);
   initialize a object for the custom service
   request for the service: request to have the velocity value;
   client1.waitForExistence();
   call of the service;
   response of the service;
 }
 
void callback /base_scan(define /base_scan message, take three values, right,left and front)

{  ROS_INFO(message to screen);
   initialize a objrct for publish velocity;
   if(the front value is minor then 0.8)
      {
        if(left value is minor than right value)
        {  turn right;}
        else
        {turn left;}
      }
   else if( left value minor than 0.55)
     {  turn right;}
   
   else if(right value minor than 0.55)
     { turn left; }
   else 
     { set the linear velocity given by the service;}
   publish velocity
}

int main (int argc, char **argv)

{  Initialize the node, setup the NodeHandle for handling the communication with the ROS system
   define subscriber for /odom and call of callback
   initialize client1 for the custom service
   define the subscriber to /base_scan
   initialize the publisher of /cmd_vel
   ros::spin();
   return 0;
}

PSEUDO-CODE Vel_controller.cpp

define publisher and service
float new_vel=0.0;
void Callback(defie /odom message)

{  ROS_INFO(message to screen );
   define object for custom service, reset service and for pub message
   aquire command from keybord;
   printf("Send 0 to reset, 1 to accelerate, 2 to decellerate \n");
   if (command == 0)
   { call reset service; 
   }
   else if(command == 1)
   { request for custom service = 1;
     call custom service;
   }
   else if(command == 2)
   {
     request for custom service= 2;
     call custom service;
   }
   }
   
int main (int argc, char **argv)

{  Initialize the node, setup the NodeHandle for handling the communication with the ROS system
   define subscriber to /odom and call callback;
   inizialise client1 for reset service;
   inizialise client2 for custom service;
   inizialise publisher to /cmd_vel;
   ros::spin();
   return 0;
}

PSEUDO-CODE car_server.cpp

float my_vel= 0.0;

bool myvel (define request and response){
        if (response == 1)
        { responset=my_vel+1.0;}
        else if (request == 2)
        { response=my_vel-1.0;}
        else if (request == 3)
        { response=my_vel;}
        my_vel=res.mult;
        return true;
}

int main(int argc, char **argv)

{  Initialize the node, setup the NodeHandle for handling the communication with the ROS system
   define service;
   ros::spin();
   return 0;
   }
