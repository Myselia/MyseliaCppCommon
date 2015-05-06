#include "include/cppcommon/IO.h"
#include "include/common/generic/FrameInputStream.h"
#include "include/common/generic/FrameOutputStream.h"

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;
using namespace com::myselia::common::generic;

void connectionHandler(boost::shared_ptr<Socket> socket);

int main(int argc, char** argv)
{
	ServerSocket serverSocket(7000);
	boost::shared_ptr<Socket> socket;

	while(true)
	{
		socket=serverSocket.accept();

		boost::thread(boost::bind(&connectionHandler, socket));
	}

	return 0;
}

void connectionHandler(boost::shared_ptr<Socket> socket)
{
	char val;
	boost::shared_ptr<InputStream> is=socket->getInputStream();
	boost::shared_ptr<OutputStream> os=socket->getOutputStream();
	FrameInputStream fis(is);
	FrameOutputStream fos(os);

	string msg="";

	for(uint i=0; i<16909060; i++)
		msg+='F';

	ByteBuffer buffer(msg);
	fos.writeFrame(buffer);
}
