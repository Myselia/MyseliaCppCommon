#ifndef INCLUDE_CPPCOMMON_MESSAGESERVICE_H_
#define INCLUDE_CPPCOMMON_MESSAGESERVICE_H_

#include <cppcommon/Exception.h>

using namespace boost;

namespace com
{
namespace myselia
{
namespace cppcommon
{

typedef boost::function<void(boost::shared_ptr<Transmission>)> TransmissionListener;

class Destination
{
	public:
	Destination(string componentId, Opcode opcode): componentId(componentId), opcode(opcode) {};

	string getComponentId() const
	{
		return componentId;
	}

	Opcode getOpcode() const
	{
		return opcode;
	}

	private:
	string componentId;
	Opcode opcode;
};

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

}
}
}

#endif /* INCLUDE_CPPCOMMON_MESSAGESERVICE_H_ */
