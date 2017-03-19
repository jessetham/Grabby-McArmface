#ifndef __SERIAL_NODE_H__
#define __SERIAL_NODE_H__

#include <sstream>
#include <ros/ros.h>
#include <serial/serial.h>
#include <serial/utils/serial_listener.h>
#include <grabby_mcarmface_ros/ServoAngleArray.h>

class SerialNode {
public:
	SerialNode();
	void angleCB( const grabby_mcarmface_ros::ServoAngleArray::ConstPtr &msg );

private:	
	void sendCommand( std::string command );
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