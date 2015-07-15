#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <std_msgs/Empty.h>

int altitude;
geometry_msgs::Twist rcTwist,autoTwist;
bool change;
std_msgs::Empty empty_msg;

 ros::Subscriber sub3;
 ros::Subscriber sub4;
 ros::Subscriber navv;
 ros::Publisher pub;
 ros::Publisher takeoff;
 ros::Publisher landing;
 ros::Publisher kill;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int decide(bool stat)
{
	if(stat == 1)
	{
		pub.publish(rcTwist);
	}
	else
	{
	    pub.publish(autoTwist);		
	}

}

int take_off(int alt)
{
	takeoff.publish(empty_msg);
}

int land()
{
	landing.publish(empty_msg);
}

int reset()
{
	kill.publish(empty_msg);
}

void ReceiveNav(const ardrone_autonomy::Navdata& MAV)
{
	altitude = MAV.altd;
	ROS_INFO(altitude);
}

void RCCallback(const geometry_msgs::Twist& arrayRC)
{
	rcTwist.linear.x = mapfloat(arrayRC.linear.x,1000,2000,-1.0,1.0);
    rcTwist.linear.y = mapfloat(arrayRC.linear.y,1000,2000,-1.0,1.0);
    rcTwist.linear.z = mapfloat(arrayRC.linear.z,1000,2000,-1.0,1.0);
    rcTwist.angular.x = 0;
    rcTwist.angular.y = 0;
    rcTwist.angular.z = mapfloat(arrayRC.angular.z,1000,2000,-1.0,1.0);
    if(arrayRC.angular.y >= 1500)
    {
    	change = 1;
    }
    else
    {
    	change = 0;
    }

    decide(change);

}


void autoCallback(const geometry_msgs::Twist& array1RC)
{
	autoTwist.linear.x = mapfloat(array1RC.linear.x,1000,2000,-1.0,1.0);
    autoTwist.linear.y = mapfloat(array1RC.linear.y,1000,2000,-1.0,1.0);
    autoTwist.linear.z = mapfloat(array1RC.linear.z,1000,2000,-1.0,1.0);
    autoTwist.angular.x = 0;
    autoTwist.angular.y = 0;
    autoTwist.angular.z = mapfloat(array1RC.angular.z,1000,2000,-1.0,1.0);

    decide(change);
}



int main(int argc , char **argv)
{
 



 ros::init(argc,argv,"auto_switch"); 
 ros::NodeHandle n,m,t;
 sub3 = n.subscribe("/rc_vals", 10, RCCallback);
 sub4 = m.subscribe("/auto_vals", 10, autoCallback);
 navv = t.subscribe("/ardrone/navdata", 1, ReceiveNav);
 pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1);
 takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff",1);
 landing = n.advertise<std_msgs::Empty>("/ardrone/land",1);
 kill = n.advertise<std_msgs::Empty>("/ardrone/reset",1);

 ros::spin();

 return 0;
}


