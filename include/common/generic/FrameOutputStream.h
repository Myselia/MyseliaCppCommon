#ifndef INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_
#define INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_

#include <arpa/inet.h>
#include <cmath>
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

class FrameOutputStream: public FilterOutputStream
{
	public:
	FrameOutputStream(boost::shared_ptr<OutputStream> fout): FrameOutputStream(fout, 4)
	{
		//Do nothing
	}

	FrameOutputStream(boost::shared_ptr<OutputStream> fout, uint sizeFieldLength): FilterOutputStream(fout), sizeFieldLength(sizeFieldLength)
	{
		if(!(sizeFieldLength==1||sizeFieldLength==2||sizeFieldLength==4))
			throw IllegalArgumentException("sizeFieldLength can only be 1 or 2 or 4 bytes");
	}

	void writeFrame(ByteBuffer& frame)
	{
		writeFrameSize(frame.getSize());
		write(frame);
	}

	private:
	uint sizeFieldLength;

	void writeFrameSize(uint frameSize)
	{
		if(frameSize>(pow(256, sizeFieldLength)))
			throw IllegalArgumentException("frameSize ("+to_string(frameSize)+") must fit in sizeFieldLength bytes ( max "+to_string(pow(256, sizeFieldLength))+")");

		union
		{
			uchar convArr[4];
			uint16_t convS;
			uint32_t convI;
		};

		if(sizeFieldLength==1)
		{
			write(frameSize);
		}
		else if(sizeFieldLength==2)
		{
			ByteBuffer buffer(sizeFieldLength);

			//Convert frame size value into byte array to send
			convS=frameSize;
			convS=htons(convS);

			buffer[0]=convArr[0];
			buffer[1]=convArr[1];

			write(buffer);
		}
		else
		{
			ByteBuffer buffer(sizeFieldLength);

			//Convert frame size value into byte array to send
			convI=frameSize;
			convI=htonl(convI);

			for(uint i=0; i<4; i++)
				buffer[i]=convArr[i];

			write(buffer);
		}
	}
};

}
}
}
}

#endif /* INCLUDE_COMMON_GENERIC_FRAMEOUTPUTSTREAM_H_ */
