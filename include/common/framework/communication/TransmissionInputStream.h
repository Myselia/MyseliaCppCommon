#ifndef INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONINPUTSTREAM_H_
#define INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONINPUTSTREAM_H_

#include "../../../common/generic/FrameInputStream.h"
#include "../../../common/communication/units/Transmission.h"
#include "../../../cppcommon/JsonTranslator.h"

using namespace boost;
using namespace com::myselia::cppcommon;
using namespace com::myselia::common::generic;
using namespace com::myselia::common::communication::units;

namespace com
{
namespace myselia
{
namespace common
{
namespace framework
{
namespace communication
{

class TransmissionInputStream: public FrameInputStream
{
	public:
	TransmissionInputStream(boost::shared_ptr<InputStream> fin): FrameInputStream(fin)
	{
		//Do nothing
	}

	boost::shared_ptr<Transmission> readTransmission()
	{
		boost::shared_ptr<ByteBuffer> buf=readFrame();
		boost::shared_ptr<JsonObject> jsonObj=boost::static_pointer_cast<JsonObject>(Json::parse(buf->getAsString()));

		return JsonTranslator::jsonToTransmission(jsonObj);
	}

	private:
};

}
}
}
}
}

#endif /* INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONINPUTSTREAM_H_ */
