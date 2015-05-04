#ifndef INCLUDE_COMMON_GENERIC_NETUTIL_H_
#define INCLUDE_COMMON_GENERIC_NETUTIL_H_

#include "../../../include/cppcommon/IO.h"

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
	static asio::mutable_buffers_1 read(InputStream* fin, int size)
	{
		char cbuf[size];
		asio::mutable_buffers_1 buffer(cbuf, sizeof(cbuf));

		int read=fin->read(buffer);

		if(read==-1)
			throw new IOException("Could not read enough bytes: end of stream.");

		return buffer;
	}

	private:
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_NETUTIL_H_ */
