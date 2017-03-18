#include <grabby_mcarmface_ros/serialNode.h>

using namespace serial;
using namespace serial::utils;

SerialNode::SerialNode( std::string port, int baud, int timeout) :
	privateNH_("~"), port_(port), baud_(baud), timeout_(timeout)
{
	serialPort_ = new Serial( port_, baud_, Timeout::simpleTimeout( 1000 ) );
	if( !serialPort_->isOpen() )
	{
		ROS_ERROR( "Serial port not working" );
	}

	// setup serial listener to listen for endline character
	serialListener_.setTokenizer( SerialListener::delimeter_tokenizer( "\n" ) );
	serialListener_.startListening( *this->serialPort_ );
}

int main( int argc, char ** argv )
{
	ros::init( argc, argv, "serialNode" );
	SerialNode SerialNode( "/dev/ttyACM0", 115200, 8000 );
	return 0;
}