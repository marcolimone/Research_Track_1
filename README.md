# Assignment_1  Marco Limone

The assignment requires our robot to execute the given path in one direction only, and in the presence of silver tokens it picks them up, moves them behind him and continues along the path.
In my code I used various versions of the "find_golden_token" function in order to detect the closest golden token frontally, to the right and to the left at the same time and to be able to detect the obstacle at a certain distance and so that the robot understands which direction it has to turn to continue along the path.The limit angles have been chosen in such a way as to have a small safety cone as small as possible in order not to detect golden tokens that are not useful, and wide enough to remain in a condition of safety and functionality that is able to take into account all possible useful golden tokens. 
It has also been implemented a control which does not make the robot go towards the silver token if it is covered by a golden token whose detection is done in a cone centered on the detection angle of the silver token passing to the function the detection angle of silver token. The width of the cone is chosen so as not to fall into limit situations that could lead the robot into a perennial loop.
There is also a check in case of non-detection of the silver token and at the end, the final code part, is dedicated to the achievement of the silver token and the way to grab it.


PSEUDO-CODE

def drive(speed, seconds):
   
    Function for setting a linear velocity



def turn(speed, seconds):
   
    Function for setting an angular velocity



def find_silver_token():    :  
   
    Function to find the closest silver token in a cone between -20 and 20 degrees

 
 
def find_golden_token():
   
    Function to find the closest golden token in a cone between -60 and 60 degrees

   

def find_golden_token_left():
   
    Function to find the closest golden token on the left of the robot in an angle between -100 and -75 degrees
                


def find_golden_token_right():
  
    Function to find the closest golden token on the right of the robot in an angle between 75 and 100 degrees

    

def find_intra_golden_token(x):
    
    Function to understand if there are golden token on the trajectori of the robot when it see a silver token

    
    
def main():
 
  d_th_golden = 0.7  "distance that the robot must keep from the closest detected golden token"
 
  d_th_silver = 0.4  "distance that the robot must have from the silver token to grab it"
 
 
  a_th = 2           "limit angle that allow the robot to go straight to the silver token"
 
  while(sempre vero):
 
   check silver token
   check golden token
   check golden token on the trajectori of the silver token
    
      
      
   if (it is closer than the safe distance) :                                      
      
      I check where the golden tokens are on the right and left
      if the distance of the golden token on the right is bigger than the one on the left :                           
         turn right
      else:
         turn left
       
   
   elif (no silver token detected or (golden token detected on the trajectori of the silver token and silver distance is bigger than golden distance)):
       
       go straight
    
    
   elif (silver distance lower than d_th_silver):                              
       
       grab silver token
       if the grab is done:                                           "
	        turn right
	        release silver token
          turn left  
       else:
          go straight
   
   
   elif (the detection angle of the silver token is between -a_th and a_th):                         
       
       go straight
       
   elif (the detection angle of the silver token is lower than -a_th):
   
       turn left
       
   elif (the detection angle of the silver token is bigger than a_th):
   
       turn right
        
  
main()
