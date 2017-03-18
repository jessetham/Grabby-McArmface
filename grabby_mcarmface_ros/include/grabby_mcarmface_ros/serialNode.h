#ifndef __SERIAL_NODE_H__
#define __SERIAL_NODE_H__

#include <ros/ros.h>
#include <serial/serial.h>
#include <serial/utils/serial_listener.h>

class SerialNode {
public:
	SerialNode( std::string port, int baud, int timeout );

private:
	serial::Serial * serialPort_;
	serial::utils::SerialListener serialListener_;

	// ROS
	ros::NodeHandle privateNH_;
	
	// Serial port
	const std::string port_;
	const int baud_;
	const int timeout_;
};

#endif