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
	//boost::shared_ptr<Transmission> transmission=boost::shared_ptr<Transmission>(new Transmission());

	ServerSocket serverSocket(7000);
	boost::shared_ptr<Socket> socket;

	socket=serverSocket.accept();
	boost::thread thread(boost::bind(&connectionHandler, socket));

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

	while(true)
	{
		cout << ">Interupting" << endl;
		thread.interrupt();

		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
	}

	return 0;
}

void connectionHandler(boost::shared_ptr<Socket> socket)
{
	boost::shared_ptr<InputStream> is=socket->getInputStream();
	boost::shared_ptr<OutputStream> os=socket->getOutputStream();

	try
	{
		int val;

		while(true)
		{
			try
			{
				val=is->read();

				if(val==-1)
					break;

				cout << val << ": '" << ((char)val) << "'" << endl;
				os->write((char)val);
			}
			catch(boost::thread_interrupted& e)
			{
				cout << "Ignoring interrupt!" << endl;
				continue;
			}
		}

		cout << "End of stream." << endl;
	}
	catch(IOException &e)
	{
		cout << "IOException: " << e.what() << endl;
	}
	catch(std::exception &e)
	{
		cout << "Exception: " << e.what() << endl;
	}


	/*boost::shared_ptr<FrameOutputStream> fos(new FrameOutputStream(os));
	TransmissionInputStream tis(is);
	TransmissionOutputStream tos(os);

	boost::shared_ptr<Transmission> transmission(new Transmission(42, "nic1", "nic2"));

	tos.writeTransmission(transmission);

	boost::shared_ptr<Transmission> transmission2=tis.readTransmission();
	string t2Json=JsonTranslator::transmissionToJson(transmission2)->toJsonString();
	cout << t2Json << endl;*/
}
