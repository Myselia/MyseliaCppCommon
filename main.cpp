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

void handleStartSandbox(boost::shared_ptr<Transmission> transmission);

int main(int argc, char** argv)
{
	string componentId="42";
	Opcode opcode(ComponentType::DAEMON, ActionType::RUNTIME, "startSandbox");
	Destination destination1(componentId, opcode);
	Destination destination2("43", opcode);

	ServerSocket serverSocket(7000);
	boost::shared_ptr<Socket> socket;
	BasicTransmissionService bts(componentId);

	bts.addListener(opcode, handleStartSandbox);

	while(true)
	{
		socket=serverSocket.accept();

		boost::shared_ptr<Transmission> t1(new Transmission(111, "us", destination1.toString()));
		boost::shared_ptr<Transmission> t2(new Transmission(222, "us", destination2.toString()));

		TransmissionOutputStream tos(socket->getOutputStream());
		tos.writeTransmission(t1);
		tos.writeTransmission(t2);

		bts.addChannel(socket);
	}

	return 0;
}

void handleStartSandbox(boost::shared_ptr<Transmission> transmission)
{
	string jsonString=JsonTranslator::transmissionToJson(transmission)->toJsonString();

	cout << "Start sandbox!" << endl;
	cout << jsonString << endl;
}
