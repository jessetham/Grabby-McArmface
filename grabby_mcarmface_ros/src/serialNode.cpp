#include <grabby_mcarmface_ros/serialNode.h>

using namespace serial;
using namespace serial::utils;

SerialNode::SerialNode() :
	privateNH_("~")
{
	privateNH_.getParam( "/serialNode/port", port_ );
	privateNH_.getParam( "/serialNode/baud", baud_ );
	privateNH_.getParam( "/serialNode/timeout", timeout_ );
	serialPort_ = new Serial( port_, baud_, Timeout::simpleTimeout( 1000 ) );
	if( !serialPort_->isOpen() )
	{
		ROS_ERROR( "Serial port not working" );
	}

	// TODO: Add service client initialization

	// setup serial listener to listen for endline character
	serialListener_.setTokenizer( SerialListener::delimeter_tokenizer( "\n" ) );
	serialListener_.startListening( *this->serialPort_ );
}

void SerialNode::listen()
{
	BufferedFilterPtr commandFilter = this->serialListener_.createBufferedFilter( SerialListener::startsWith( "," ) );
	while( ros::ok() )
	{
		std::string command = commandFilter->wait( 1000 );
		if( command.empty() )
		{
			continue;
		}
		else
		{
			if( !command.compare( ",listen" ) )
			{
				// send command to microphone and wit.ai node
				// recieve servo angles and send to arduino
			}
			else if( !command.compare( ",record" ) )
			{
				// setup serial port to recieve angles
				ROS_INFO( "Contacting voice service" );
				callVoiceService( command.c_str() );
			}
		}
		ros::spinOnce();
	}
}

void SerialNode::angleCB( const grabby_mcarmface_ros::ServoAngleArray::ConstPtr &msg )
{
	std::stringstream ss;
	ss << "s1" << msg->servoAngle1 << "s2" << msg->servoAngle2 << "s3" << msg->servoAngle3\
		<< "s4" << msg->servoAngle4 << "s5" << msg->servoAngle5 << "\n";
	sendCommand( ss.str() );
}

void SerialNode::sendCommand( std::string command )
{
	BufferedFilterPtr replyFilter = this->serialListener_.createBufferedFilter( SerialListener::startsWith( "," ) );
	this->serialPort_->write( command );
	std::string reply = replyFilter->wait( timeout_ );
	if( reply.empty() )
	{
		ROS_ERROR( "I didn't hear anything" );
	}
	else
	{
		ROS_INFO("I heard %s", reply.c_str() );
	}
}

void SerialNode::callVoiceService( std::string command )
{
	
}

int main( int argc, char ** argv )
{
	ros::init( argc, argv, "serialNode" );
	SerialNode serialNode;
	serialNode.listen();
	
	return 0;
}