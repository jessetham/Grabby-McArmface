#ifndef __SERIAL_NODE_H__
#define __SERIAL_NODE_H__

#include <ros/ros.h>
#include <serial/serial.h>
#include <serial/utils/serial_listener.h>

class SerialNode {
public:
	SerialNode();

private:
	// ROS 
	ros::NodeHandle privateNH_;
	
	// Serial port
	serial::Serial * serialPort_;
	serial::utils::SerialListener serialListener_;
	std::string port_;
	int baud_;
	int timeout_;
};

#endif