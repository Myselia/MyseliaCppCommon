#ifndef INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_
#define INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_

#include <arpa/inet.h>
#include <cppcommon/IO.h>
#include <common/generic/NetUtil.h>

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
	FrameInputStream(boost::shared_ptr<InputStream> fin): FrameInputStream(fin, 4)
	{
		//Do nothing
	}

	FrameInputStream(boost::shared_ptr<InputStream> fin, uint sizeFieldLength): FilterInputStream(fin), sizeFieldLength(sizeFieldLength)
	{
		if(!(sizeFieldLength==1||sizeFieldLength==2||sizeFieldLength==4))
			throw IllegalArgumentException("sizeFieldLength can only be 1 or 2 or 4 bytes");
	}

	boost::shared_ptr<ByteBuffer> readFrame()
	{
		uint frameSize=readFrameSize();

		return NetUtil::read(this, frameSize);
	}

	private:
	uint sizeFieldLength;

	uint readFrameSize()
	{
		union
		{
			uchar convArr[4];
			uint16_t convS;
			uint32_t convI;
		};

		if(sizeFieldLength==1)
		{
			boost::shared_ptr<ByteBuffer> buffer=NetUtil::read(this, 1);
			return (*buffer)[0];
		}
		else if(sizeFieldLength==2)
		{
			boost::shared_ptr<ByteBuffer> buffer=NetUtil::read(this, 2);

			uchar* data=(uchar*)buffer->getData();

			//Convert read bytes into ushort using the union
			for(uint i=0; i<2; i++)
				convArr[i]=data[i];

			convS=ntohs(convS);

			return convS;
		}
		else
		{
			boost::shared_ptr<ByteBuffer> buffer=NetUtil::read(this, 4);

			uchar* data=(uchar*)buffer->getData();

			//Convert read bytes into uint using the union
			for(uint i=0; i<4; i++)
				convArr[i]=data[i];

			convI=ntohl(convI);

			return convI;
		}
	}
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_FRAMEINPUTSTREAM_H_ */
