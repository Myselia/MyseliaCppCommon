#ifndef INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_
#define INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_

#include <arpa/inet.h>
#include <cmath>
#include "../../../include/cppcommon/IO.h"
#include "../../../include/common/generic/NetUtil.h"

using namespace boost;
using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace common
{
namespace generic
{

class FrameOutputStream: public FilterOutputStream
{
	public:
	FrameOutputStream(boost::shared_ptr<OutputStream> fout);
	FrameOutputStream(boost::shared_ptr<OutputStream> fout, uint sizeFieldLength);

	void writeFrame(ByteBuffer& frame);

	private:
	uint sizeFieldLength;

	void writeFrameSize(uint frameSize);
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_ */
