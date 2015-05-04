#ifndef INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_
#define INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_

#include <arpa/inet.h>
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

class FrameInputStream: public FilterInputStream
{
	public:
	FrameInputStream(boost::shared_ptr<InputStream> fin);
	FrameInputStream(boost::shared_ptr<InputStream> fin, uint sizeFieldLength);

	uint readFrameSize();
	boost::shared_ptr<ByteBuffer> readFrame();

	private:
	uint sizeFieldLength;
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_ */
