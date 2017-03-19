#include <grabby_mcarmface_ros/serialNode.h>

using namespace serial;
using namespace serial::utils;

SerialNode::SerialNode() :
	privateNH_("~")
{
	privateNH_.getParam( "serialNode/port", port_ );
	privateNH_.getParam( "serialNode/baud", baud_ );
	privateNH_.getParam( "serialNode/timeout", timeout_ );
	serialPort_ = new Serial( port_, baud_, Timeout::simpleTimeout( 1000 ) );
	if( !serialPort_->isOpen() )
	{
		ROS_ERROR( "Serial port not working" );
	}

	// setup serial listener to listen for endline character
	serialListener_.setTokenizer( SerialListener::delimeter_tokenizer( "\n" ) );
	serialListener_.startListening( *this->serialPort_ );

	std::string angleTopic;
	privateNH_.getParam( "serialNode/angleTopic", angleTopic );
	angleSub_ = privateNH_.subscribe( angleTopic, 1, &SerialNode::angleCB, this );
	ROS_INFO( "Subscribed to to angle topic" );
}

void SerialNode::angleCB( const grabby_mcarmface_ros::ServoAngleArray::ConstPtr &msg )
{
	char inputBuffer[MAX_MSG_SIZE];
	sprintf( inputBuffer, "s1%ds2%ds3%ds4%ds5%d\n", msg->servoAngle1, msg->servoAngle2,
		msg->servoAngle3, msg->servoAngle4, msg->servoAngle5 );
}

int main( int argc, char ** argv )
{
	ros::init( argc, argv, "serialNode" );
	SerialNode SerialNode();
	return 0;
}