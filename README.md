# Assignment_2
In this second assignment I have a robot which should move autonomously inside a circuit and I have to control it with ROS, in a special way I need to have a node that control the movement of the robot in the circuit, and another node that allow the user to increase or decrease and to reset the position of the robot. 
In my code I use two node called "carcontroller_node" and "Velcontroller_node", and a custom service called "server_vel_node".These three nodes allow me respectively to manage the laser scans of the robot so as not to crash (carcontroller_node), to manage the user inputs for acceleration, deceleration and reset of the robot (Velcontroller_node) and the server (server_vel_node) to manage the new calculated speed and make it available for all nodes, in particular for the carcontroller_node.

carcontrollre_node
This node executes the instructions present in the car_controller.cpp file which in turn is located in the src folder of the pacakge car_controller of my workspace. In the code I use a publisher who publishes the speed values that my robot must have and which are managed in such a way as not to make the robot crash into obstacles. I use also two subscriber: 
the first one gives me the laser scan values in order to detect obstacles and allow the robot to avoid them, in a special way in his callback there is all the logic for the right movement of the robot and for the publication of the correct speed value that the robot must keep;
the second one allows us to have data on the speed at which the robot is traveling, in particular its callback allows us to call up the server of our custom service and set the desired speed.

Velcontroller_node
This node executes the instructions present in the Vel_controller.cpp file which in turn is located in the src folder of the package vel_controller of my workspace. In the code I use a subscriber to /odom topic and in its callback I acquire the user command from the keyboard, and depending on the command I call two services, one for reset the robot position and one for the acceleration or deceleration of the robot.

server_vel_node
This node is entirely organized in the my_car_srv package which contains the srv folder, where is located the Velocity_car.srv file  which contains the structure of the message that the servic is going to manage, and the src folder where is located the file car_server.cpp which contains the istruction that performs the service.


