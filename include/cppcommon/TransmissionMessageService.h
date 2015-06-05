/*
 * TransmissionMessageService.h
 *
 * Implementation of MessageService.h but for Transmission protocol.
 */

#ifndef INCLUDE_CPPCOMMON_TRANSMISSIONMESSAGESERVICE_H_
#define INCLUDE_CPPCOMMON_TRANSMISSIONMESSAGESERVICE_H_

#include <cppcommon/MessageService.h>
#include <cppcommon/JsonTranslator.h>
#include <common/communication/units/Transmission.h>

using namespace boost;
using namespace com::myselia::common::communication::units;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class TransmissionMessage: public DefaultMessage
{
	public:
	TransmissionMessage(boost::shared_ptr<Transmission> transmission): transmission(transmission) {};

	virtual boost::shared_ptr<ByteBuffer> getMessageData()
	{
		string jsonString=JsonTranslator::transmissionToJson(transmission)->toJsonString();
		return boost::shared_ptr<ByteBuffer>(new ByteBuffer(jsonString));
	}

	boost::shared_ptr<Transmission> getTransmission()
	{
		return transmission;
	}

	private:
	boost::shared_ptr<Transmission> transmission;
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_TRANSMISSIONMESSAGESERVICE_H_ */
