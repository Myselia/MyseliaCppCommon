#ifndef INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONOUTPUTSTREAM_H_
#define INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONOUTPUTSTREAM_H_

#include <common/generic/FrameOutputStream.h>
#include <common/communication/units/Transmission.h>
#include <cppcommon/JsonTranslator.h>

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

class TransmissionOutputStream: public FrameOutputStream
{
	public:
	TransmissionOutputStream(boost::shared_ptr<OutputStream> fout): FrameOutputStream(fout)
	{
		//Do nothing
	}

	void writeTransmission(boost::shared_ptr<Transmission> transmission)
	{
		string jsonString=JsonTranslator::transmissionToJson(transmission)->toJsonString();
		ByteBuffer buf(jsonString);

		writeFrame(buf);
	}

	private:
};

}
}
}
}
}

#endif /* INCLUDE_COMMON_FRAMEWORK_COMMUNICATION_TRANSMISSIONOUTPUTSTREAM_H_ */
