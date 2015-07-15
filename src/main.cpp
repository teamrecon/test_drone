#include<ros/ros.h>
#include<std_msgs/String.h>
#include<sstream>


int main(int argc , char **argv)
{
 ros::init(argc,argv,"Sender"); 
 ros::NodeHandle n;
 ros::Publisher chatter1=n.advertise<std_msgs::String>("chatter",100);
 ros::Rate loop_rate(1.0);
 int count =0;
 while(ros::ok())
 {
  std_msgs::String msg;
  std::stringstream ss;
  ss << "Hello"<< count;
  msg.data =ss.str();
  ROS_INFO("%s",msg.data.c_str());
  chatter1.publish(msg);
  ros::spinOnce();
  loop_rate.sleep();
  ++count;
  }
 return 0;
}
