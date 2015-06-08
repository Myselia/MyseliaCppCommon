#ifndef INCLUDE_CPPCOMMON_TRANSMISSIONSERVICE_H_
#define INCLUDE_CPPCOMMON_TRANSMISSIONSERVICE_H_

#include <cppcommon/Opcode.h>
#include <common/framework/communication/TransmissionInputStream.h>
#include <common/framework/communication/TransmissionOutputStream.h>

using namespace boost;
using namespace com::myselia::common::framework::communication;
using namespace com::myselia::common::communication::units;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class BufferedTIS
{
	public:
	BufferedTIS(boost::shared_ptr<TransmissionInputStream> tis)
	{
		//
	}

	private:
	boost::shared_ptr<TransmissionInputStream> tis;
	boost::thread readTh;
	std::list<boost::shared_ptr<Transmission>> buffer;

	void read()
	{
		//
	}
};

class TransmissionChannel
{
	public:
	TransmissionChannel(boost::shared_ptr<Channel> channel)
	{
		in=boost::shared_ptr<TransmissionInputStream>(new TransmissionInputStream(channel->getInputStream()));
		out=boost::shared_ptr<TransmissionOutputStream>(new TransmissionOutputStream(channel->getOutputStream()));
	};

	boost::shared_ptr<TransmissionInputStream> getTransmissionInputStream()
	{
		return in;
	}

	boost::shared_ptr<TransmissionOutputStream> getTransmissionOutputStream()
	{
		return out;
	}

	private:
	boost::shared_ptr<TransmissionInputStream> in;
	boost::shared_ptr<TransmissionOutputStream> out;
};

typedef boost::function<void(boost::shared_ptr<Transmission>)> TransmissionListener;

class Destination
{
	public:
	Destination(string componentId, Opcode opcode): componentId(componentId), opcode(opcode) {};

	static Destination fromString(string str)
	{
		vector<string> tokens;

		GenericUtil::tokenize(str, tokens, ""+delimiter);

		if(tokens.size()!=2)
			throw IllegalArgumentException("Invalid string: \""+str+"\"");

		return Destination(tokens[0], Opcode::fromString(tokens[1]));
	}

	string getComponentId() const
	{
		return componentId;
	}

	Opcode getOpcode() const
	{
		return opcode;
	}

	string toString() const
	{
		return componentId+delimiter+opcode.toString();
	}

	private:
	static char delimiter;

	string componentId;
	Opcode opcode;
};

char Destination::delimiter=':';

class TransmissionSender
{
	public:
	virtual ~TransmissionSender() {};

	virtual void send(Destination destination, boost::shared_ptr<Transmission> transmission) = 0;
};

class TransmissionReceiver
{
	public:
	virtual ~TransmissionReceiver() {};

	virtual void addListener(Opcode opcode, TransmissionListener listener) = 0;
};

class TransmissionService: public TransmissionSender, public TransmissionReceiver
{
	public:
	virtual ~TransmissionService() {};
};

class BasicTransmissionService: public TransmissionService
{
	public:
	BasicTransmissionService(string componentId): componentId(componentId)
	{
		//Do nothing
	}

	void addChannel(boost::shared_ptr<Channel> channel)
	{
		boost::shared_ptr<TransmissionChannel> transmissionChannel=boost::shared_ptr<TransmissionChannel>(new TransmissionChannel(channel));

		channelsMutex.lock();
		channels.insert(transmissionChannel);
		channelsMutex.unlock();
	}

	void send(Destination destination, boost::shared_ptr<Transmission> transmission)
	{
		//TODO
	}

	void addListener(Opcode opcode, TransmissionListener listener)
	{
		listeners[opcode]=listener;
	}

	private:
	string componentId;
	std::set<boost::shared_ptr<TransmissionChannel>> channels;
	boost::mutex channelsMutex;
	std::set<int> receivedTransmissions;
	std::unordered_map<Opcode, TransmissionListener> listeners;
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_TRANSMISSIONSERVICE_H_ */
