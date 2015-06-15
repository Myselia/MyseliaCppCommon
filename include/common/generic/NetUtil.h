#ifndef INCLUDE_COMMON_GENERIC_NETUTIL_H_
#define INCLUDE_COMMON_GENERIC_NETUTIL_H_

#include <cppcommon/JavaIO.h>

using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace common
{
namespace generic
{

class NetUtil
{
	public:
	static boost::shared_ptr<ByteBuffer> read(InputStream* fin, int size)
	{
		boost::shared_ptr<ByteBuffer> buffer(new ByteBuffer(size));

		int read=fin->read(*buffer);

		if(read==-1)
			throw IOException("Could not read enough bytes: end of stream.");

		return buffer;
	}

	private:
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_NETUTIL_H_ */
