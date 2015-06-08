#include <cppcommon/IO.h>
#include <common/framework/communication/TransmissionInputStream.h>
#include <common/framework/communication/TransmissionOutputStream.h>
#include <common/generic/FrameOutputStream.h>
#include <cppcommon/TransmissionService.h>

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;
using namespace com::myselia::common::generic;
using namespace com::myselia::common::framework::communication;

void connectionHandler(boost::shared_ptr<Socket> socket);

int main(int argc, char** argv)
{
	boost::shared_ptr<Transmission> transmission=boost::shared_ptr<Transmission>(new Transmission());

	/*
	ServerSocket serverSocket(7000);
	boost::shared_ptr<Socket> socket;

	while(true)
	{
		socket=serverSocket.accept();

		boost::thread(boost::bind(&connectionHandler, socket));
	}

	return 0;*/
}

void connectionHandler(boost::shared_ptr<Socket> socket)
{
	boost::shared_ptr<InputStream> is=socket->getInputStream();
	boost::shared_ptr<OutputStream> os=socket->getOutputStream();
	boost::shared_ptr<FrameOutputStream> fos(new FrameOutputStream(os));
	TransmissionInputStream tis(is);
	TransmissionOutputStream tos(os);

	boost::shared_ptr<Transmission> transmission(new Transmission(42, "nic1", "nic2"));

	tos.writeTransmission(transmission);

	boost::shared_ptr<Transmission> transmission2=tis.readTransmission();
	string t2Json=JsonTranslator::transmissionToJson(transmission2)->toJsonString();
	cout << t2Json << endl;
}
