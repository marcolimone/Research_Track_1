#include "ros/ros.h"
#include "my_car_srv/Velocity_car.h"

float my_vel= 0.0;                     //define and initialize a global variable that contains the velocity of our robot

bool myvel (my_car_srv::Velocity_car::Request &req, my_car_srv::Velocity_car::Response &res){
        if (req.velocity == 1)                          //request 1 add a velocity unit to our velocity
        {
          res.mult=my_vel+1.0;
        }
        else if (req.velocity == 2)                      //request 2 substract a velocity unit to our velocity
        {
          res.mult=my_vel-1.0;        
        }
        else if (req.velocity == 3)                     //request 3 give back the current velocity
        {
          res.mult=my_vel;                
        }
        my_vel=res.mult;
        return true;
}

int main(int argc, char **argv)
{
   ros::init(argc, argv, "server");
   ros::NodeHandle n3;
   ros::ServiceServer service= n3.advertiseService("/velocity_car", myvel);
   ros::spin();
   return 0;
   }
