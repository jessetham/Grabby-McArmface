#ifndef __SERIAL_NODE_H__
#define __SERIAL_NODE_H__

#define MAX_MSG_SIZE 30

#include <ros/ros.h>
#include <serial/serial.h>
#include <serial/utils/serial_listener.h>
#include <grabby_mcarmface_ros/ServoAngleArray.h>

class SerialNode {
public:
	SerialNode();
	void angleCB( const grabby_mcarmface_ros::ServoAngleArray::ConstPtr &msg );

private:	
	// ROS 
	ros::NodeHandle privateNH_;
	ros::Subscriber angleSub_;
	
	// Serial port
	serial::Serial * serialPort_;
	serial::utils::SerialListener serialListener_;
	std::string port_;
	int baud_;
	int timeout_;
};

#endif