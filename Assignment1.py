from __future__ import print_function
import time
from sr.robot import *


R = Robot()
""" instance of the class Robot"""

def drive(speed, seconds):
    """
    Function for setting a linear velocity

    Args: speed (int): the speed of the wheels
	  seconds (int): the time interval
    """
    R.motors[0].m0.power = speed
    R.motors[0].m1.power = speed
    time.sleep(seconds)
    R.motors[0].m0.power = 0
    R.motors[0].m1.power = 0

def turn(speed, seconds):
    """
    Function for setting an angular velocity
    
    Args: speed (int): the speed of the wheels
	  seconds (int): the time interval
    """
    R.motors[0].m0.power = speed
    R.motors[0].m1.power = -speed
    time.sleep(seconds)
    R.motors[0].m0.power = 0
    R.motors[0].m1.power = 0

def find_silver_token_near():    
    """
    Function to find a silver token at a very low distance

    Returns:
  dist (float): distance of the closest silver token (-1 if no silver token is detected)
  rot_y (float): angle between the robot and the silver token (-1 if no silver token is detected)
    """
    dist = 1

    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_SILVER and -100 < token.rot_y < +100:
            dist=token.dist
            rot_y=token.rot_y
    if dist == 1:
        return -1, -1
    else:
        return dist, rot_y
 

def find_silver_token():    
    """
    Function to find the closest silver token in a cone in front of the robot

    Returns:
  dist (float): distance of the closest silver token (-1 if no silver token is detected)
  rot_y (float): angle between the robot and the silver token (-1 if no silver token is detected)
    """
    dist = 7

    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_SILVER and -20 < token.rot_y < 20:
            dist=token.dist
            rot_y=token.rot_y
    if dist == 7:
        return -1, -1
    else:
        return dist, rot_y
 


def find_golden_token():
    """
    Function to find the closest golden token

    Returns:
  dist (float): distance of the closest golden token (-1 if no golden token is detected)
  rot_y (float): angle between the robot and the golden token (-1 if no golden token is detected)
    """
    dist=100
    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_GOLD and -60 < token.rot_y < 60:
            dist=token.dist
            rot_y=token.rot_y
    if dist==100:
        return -1, -1
    else:
        return dist , rot_y

def find_golden_token_left():
    """
    Function to find the closest golden token on the left of the robot in an angle between -100 and -75

    Returns:
  dist (float): distance of the closest golden token (-1 if no golden token is detected)
  rot_y (float): angle between the robot and the golden token (-1 if no golden token is detected)
    """ 
    dist=1000
    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_GOLD and -100 < token.rot_y < -75: 
            dist=token.dist
            rot_y=token.rot_y
    if dist==1000:
        return -1, -1
    else:
        return dist , rot_y    

def find_golden_token_right():
    """
    Function to find the closest golden token on the right of the robot in an angle between 75 and 100

    Returns:
  dist (float): distance of the closest golden token (-1 if no golden token is detected)
  rot_y (float): angle between the robot and the golden token (-1 if no golden token is detected)
    """
    dist=1000
    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_GOLD and 75 < token.rot_y < 100:
            dist=token.dist
            rot_y=token.rot_y
    if dist==1000:
        return -1, -1
    else:
        return dist , rot_y

def find_intra_golden_token(x):
    """
    Function to understand if there are golden token on the trajectori of the robot when it see a silver token

    Returns:
  dist (float): distance of the closest golden token (-1 if no golden token is detected)
  rot_y (float): angle between the robot and the golden token (-1 if no golden token is detected)
    """
    dist=100
    for token in R.see():
        if token.dist < dist and token.info.marker_type is MARKER_TOKEN_GOLD and x-30 < token.rot_y < x+30:
            dist=token.dist
            rot_y=token.rot_y
    if dist==100:
        return -1, -1
    else:
        return dist , rot_y    
    
def main():
 
  d_th_golden = 0.7  #"""distance that the robot must keep from the closest detected golden token"""
 
  d_th_silver = 0.4  #"""distance that the robot must have from the silver token to grab it"""
 
 
  a_th = 2           #"""limit angle that allow the robot to go straight to the silver token"""
 
  while(1):
 
   d_silver , rot_y_silver = find_silver_token()
   d_golden , rot_y_golden = find_golden_token()
   dist_golden , rot_y_golden_intra = find_intra_golden_token (rot_y_silver)
   d_silver_near , rot_y_silver_near = find_silver_token_near()
   
   if d_silver_near != -1 :                            # """check done to see any very close silver tokens not seen"""
      
      if d_silver_near < d_th_silver:                  # """if the robot is close enough to the token, we try grab it"""
         print("Found near me!")
         if R.grab():                                  # """if the robot grab the token, the robot move forward and on the right, it release the token, and it go back to the initial position"""            
            print("Gotcha!")
	    turn(30, 2)
	    print("Put it behind me")
	    R.release()
	    print("Realise it")
            turn(-30,2)  
         
                
         
      elif -a_th<= rot_y_silver_near <= a_th:                        #"""if the robot is well aligned with the token, we go forward"""
         print("Oh, this way")
         drive(50, 0.02)
      elif rot_y_silver_near < -a_th:                                # """if the robot is not well aligned with the token, we move it on the left or on the right"""
         print("Left a bit")
         turn(-2, 0.5)
      elif rot_y_silver_near > a_th:
         print("Right a bit")
         turn(+2, 0.5)
    
      
   elif d_golden<d_th_golden  :                                       #"""control of the distance beetween robot and closest detected golden token"""
      d_golden_right, rot_y_golden_r = find_golden_token_right()
      d_golden_left , rot_y_golden_l = find_golden_token_left()
      if d_golden_right > d_golden_left :                           # """control for the direction in which the robot has to turn"""
          print("turn right") 
          print(d_golden_right)
          print(d_golden_left)
          print(rot_y_golden_r)
          print(rot_y_golden_l)
          turn(7,1)
      else:
          print("turn left")
          print(d_golden_right)
          print(d_golden_left)
          print(rot_y_golden_r)
          print(rot_y_golden_l)
          turn(-7,1)  
   
   elif d_silver == -1 or (dist_golden != -1 and d_silver > dist_golden):
       print("nothing found")
       drive(50,0.02)  
    
   
                
   
   elif -a_th<= rot_y_silver <= a_th:                        #"""if the robot is well aligned with the token, we go forward"""
       print("Ah, that'll do.")
       drive(50, 0.02)
   elif rot_y_silver < -a_th:                                # """if the robot is not well aligned with the token, we move it on the left or on the right"""
       print("Left a bit...")
       turn(-2, 0.5)
   elif rot_y_silver > a_th:
       print("Right a bit...")
       turn(+2, 0.5)
        
     
    
     
     
     
     


main()	
