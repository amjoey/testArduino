#include <ros.h>
#include <geometry_msgs/Twist.h>

//////////////////////////////////////////////////////////////////////////////////////

float WheelSeparation = 0.6;
 
//sudo vars//////////////////////////////////////////////////////////////////////////

double WCS[2] = {0,0};

//ROS variables//////////////////////////////////////////////////////////////////////
ros::NodeHandle nh;
////ROS publisher
geometry_msgs::Twist debug_msg;
ros::Publisher Debug ("debug", &debug_msg);
//ROS subscriber

void messageCb( const geometry_msgs::Twist& CVel){
  //geometry_msgs::Twist twist = twist_msg;   
    double vel_x = CVel.linear.x;
    double vel_th = CVel.angular.z;
    double right_vel = 0.0;
    double left_vel = 0.0;

    // turning
    if(vel_x == 0){  
        right_vel = vel_th * WheelSeparation / 2.0;
        left_vel = (-1) * right_vel;
    }
    // forward / backward
    else if(vel_th == 0){ 
        left_vel = right_vel = vel_x;
    }
    // moving doing arcs
    else{ 
        left_vel = vel_x - vel_th * WheelSeparation / 2.0;
        right_vel = vel_x + vel_th * WheelSeparation / 2.0;
    }
    //write new command speeds to global vars 
    WCS[0] = left_vel;
    WCS[1] = right_vel;
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );


void setup()
{
 // nh.getHardware()->setBaud(115200);;
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(Debug);
}

void loop()
{
    nh.spinOnce();
    debug_msg.linear.x = WCS[0];
    debug_msg.linear.y = 0;
    debug_msg.linear.z = 0;
    debug_msg.angular.x = WCS[1];
    debug_msg.angular.y = 0;
    debug_msg.angular.z= 0;

    Debug.publish(&debug_msg);
  delay(3);
}
