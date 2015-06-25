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

class TransmissionSender
{
	public:
	virtual ~TransmissionSender() {};

	virtual void send(boost::shared_ptr<Transmission> transmission) = 0;
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

	~BasicTransmissionService()
	{
		channelHandlers.interrupt_all();
		channelHandlers.join_all();
	}

	void addChannel(boost::shared_ptr<Channel> channel)
	{
		boost::shared_ptr<TransmissionChannel> transmissionChannel=boost::shared_ptr<TransmissionChannel>(new TransmissionChannel(channel));

		channelsMutex.lock();
		channels.insert(transmissionChannel);
		channelsMutex.unlock();


		boost::thread* thread=new boost::thread(boost::bind(&BasicTransmissionService::handleChannel, this, transmissionChannel));
		channelHandlers.add_thread(thread);
	}

	void send(boost::shared_ptr<Transmission> transmission)
	{
		Destination destination=Destination::fromString(transmission->getTo());

		//Set Transmission ID
		transmission->setId((uint)GenericUtil::generateRandomPositiveInt());

		//Set only the component ID (opcode should have been set by the sender).
		boost::shared_ptr<Destination> from;

		try
		{
			from=boost::make_shared<Destination>(Destination::fromString(transmission->getFrom()));
		}
		catch(IllegalArgumentException& e )
		{
			cout << "Error: sender did not set its address correctly. Address: '" << transmission->getFrom() << "'" << endl;
			throw e;
		}

		from->setComponentId(componentId);
		transmission->setFrom(from->toString());

		/* Here you would normally use routing to determine through which channel
		 * to send the Transmission. The current implementation just sends it to all channels.
		 */

		channelsMutex.lock();

		boost::shared_ptr<TransmissionOutputStream> tos;

		for(boost::shared_ptr<TransmissionChannel> channel: channels)
		{
			tos=channel->getTransmissionOutputStream();
			tos->writeTransmission(transmission);
		}

		channelsMutex.unlock();
	}

	void addListener(Opcode opcode, TransmissionListener listener)
	{
		listenersMutex.lock();
		listeners[opcode]=listener;
		listenersMutex.unlock();
	}

	private:
	string componentId;
	std::set<boost::shared_ptr<TransmissionChannel>> channels;
	boost::mutex channelsMutex;
	std::set<int> receivedTransmissions;
	boost::mutex receivedTransmissionsMutex;
	std::unordered_map<Opcode, TransmissionListener> listeners;
	boost::mutex listenersMutex;
	boost::thread_group channelHandlers;

	void handleChannel(boost::shared_ptr<TransmissionChannel> transmissionChannel)
	{
		boost::shared_ptr<TransmissionInputStream> tis=transmissionChannel->getTransmissionInputStream();
		boost::shared_ptr<Transmission> transmission;

		while(true)
		{
			try
			{
				transmission=tis->readTransmission();
			}
			catch(IOException& e)
			{
				channelsMutex.lock();
				channels.erase(transmissionChannel);
				channelsMutex.unlock();
				return;
			}
			catch(boost::thread_interrupted& e)
			{
				return;
			}

			receivedTransmissionsMutex.lock();
			//If we already received that Transmission
			if(receivedTransmissions.find(transmission->getHeader()->getId())!=receivedTransmissions.end())
			{
				receivedTransmissionsMutex.unlock();
				continue;
			}

			receivedTransmissions.insert(transmission->getHeader()->getId());
			receivedTransmissionsMutex.unlock();

			boost::shared_ptr<Destination> to;

			try
			{
				to=boost::shared_ptr<Destination>(boost::make_shared<Destination>(Destination::fromString(transmission->getHeader()->getTo())));
			}
			catch(GenericException& e)
			{
				cout << "BasicTransmissionService: error parsing Transmission destination: \""+transmission->getHeader()->getTo()+"\", error: " << e.getMessage() << endl;
				continue;
			}

			//If the transmission targets this component.
			if(to->getComponentId()==componentId)
			{
				listenersMutex.lock();

				//If there is a listener that listens for this opcode.
				if(listeners.find(to->getOpcode())!=listeners.end())
				{
					//Call listener
					TransmissionListener listener=listeners[to->getOpcode()];

					listenersMutex.unlock();

					listener(transmission);
				}
			}
			else
			{
				//The transmission does not target this component. Send the transmission to the corresponding channel.
				send(transmission);
			}
		}
	}
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_TRANSMISSIONSERVICE_H_ */
