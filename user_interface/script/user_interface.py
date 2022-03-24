#!/usr/bin/env python


import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from mode_srv.srv import Mode
from first_mode_srv.srv import Firstmode
from controller_srv.srv import Controller
from nav_msgs.msg import Odometry




def userClk (msg):
  
  flag = rospy.get_param("/flag")
   
  if (flag == 1):
  
           print("Choose an action:\n"              
                        "1 - Mode 1\n"
                        "2 - Mode 2\n"
                        "3 - Mode 3\n")
   
           
           command = int(input())
   
           if (command == 1):
   
                       rospy.wait_for_service("/mode")
                       client1 = rospy.ServiceProxy("/mode", Mode)
                       client1(1, 1)
                       rospy.set_param("/mode1", 1)
                       rospy.set_param("/mode2", 0)
                       rospy.set_param("/mode3", 0)
   
           elif (command == 2):
   
                       rospy.wait_for_service("/mode")
                       client1 = rospy.ServiceProxy("/mode", Mode)
                       client1(1, 2)
                       rospy.set_param("/mode1", 0)
                       rospy.set_param("/mode2", 1)
                       rospy.set_param("/mode3", 0)
   
   
           elif (command == 3):
   
                       rospy.wait_for_service("/mode")
                       client1 = rospy.ServiceProxy("/mode", Mode)
                       client1(1, 3)
                       rospy.set_param("/mode1", 0)
                       rospy.set_param("/mode2", 0)
                       rospy.set_param("/mode3", 1)
   
          
           rospy.set_param("/flag", 0)
   
   
  mode1=rospy.get_param("/mode1")
  mode2=rospy.get_param("/mode2")
  mode3=rospy.get_param("/mode3")
  if(mode1 == 1):
    
           
     
           print("Insert a new position \n")
           print("New x position: \n")
           x = float(input())
           print("New y position: \n")
           y = float(input())
     
           rospy.wait_for_service("/first_mode")
           client2 = rospy.ServiceProxy("/first_mode", Firstmode)
           client2(1, x, y)
           
           
     
           print("Choose an action:\n"              
                  "1 - go on with this mode\n"
                  "2 - change mode 2\n")
           command = int(input())
           if (command == 2):
     
                   rospy.set_param("/flag", 1)
           
     
   
   
  
   
  if(mode2 == 1):
          
           
           print("\nCommands:\n"
                  "1 - Go straight\n"
                  "2 - Stop\n"
                  "3 - Turn right\n"
                  "4 - Turn left\n"
                  "5 - Change mode\n"
            
                  )
            
           comm = int(input())
           if (comm == 5):
     
                  rospy.set_param("/flag", 1)
       
           else:
     
                  rospy.wait_for_service("/controller")
                  client3 = rospy.ServiceProxy("/controller", Controller)
                  client3(1, comm)
                  
     
   
   
  if(mode3 == 1):
   
           
           print("\nCommands:\n"
                  "1 - Go straight\n"
                  "2 - Stop\n"
                  "3 - Turn right\n"
                  "4 - Turn left\n"
                  "5 - Change mode\n"
                  )
     
           comm = int(input())
     
           if (comm == 5):
     
                  rospy.set_param("/flag", 1)
       
           else:
     
                  rospy.wait_for_service("/controller")
                  client3 = rospy.ServiceProxy("/controller", Controller)
                  client3(1, comm)
     
   
   
 




def user ():


   #Initialize the node, setup the NodeHandle for handling the communication with the ROS system
   rospy.init_node("user_subscriber")
   rospy.Subscriber("/odom", Odometry, userClk)
   
   
   
   rospy.spin()

if __name__ == '__main__':
            user()

