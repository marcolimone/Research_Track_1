# Assignment_3 Marco Limone

In the third assignment I have a robot located on a map that it doesn't know about. Thanks to the lasers the robot is supplied with, it is able to see obstacles and build the map in which it acts as it explores it.
The assignment consist in the development of a software architecture for the control of a mobile robot in the environment. The software will rely on the move_base
and gmapping packages for localizing the robot and plan the motion. The user can choose between the following three control modes for the robot:
- 1) autonomously reach a x,y coordinate inserted by the user
- 2) let the user drive the robot with the keyboard
- 3) let the user drive the robot assisting them to avoid collisions                                                                   

To achieve this goal, a node has been created which acts as an interface for the user and at the same time manages the movements of the robot. In this node I initialize two subscriber. The first subscriber is subscribed to the /odom topic and its callback contains the part of the user interface of the three mode. For the first mode there is the request of the coordinates that the robot must achieve and than them are sent to the topic move_base/goal. For the second mode the user have to sent a command depending of what he want that the robot will do. Depending on the command received, are set linear and angular velocity publishing them by the topic /cmd_vel. For the third is different because in the first callback there is only the acquisition of the command. In the second callback, which subscriber is subscribed to the laser scan topic, there is the check of the distance of the walls and there is the logic of the assistent drive that let to avoid collisions. 
