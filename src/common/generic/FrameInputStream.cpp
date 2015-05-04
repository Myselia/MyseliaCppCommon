#include "../../../include/common/generic/FrameInputStream.h"

using namespace com::myselia::common::generic;

FrameInputStream::FrameInputStream(boost::shared_ptr<InputStream> fin): FrameInputStream(fin, 4)
{
	//Do nothing
}

FrameInputStream::FrameInputStream(boost::shared_ptr<InputStream> fin, uint sizeFieldLength): FilterInputStream(fin), sizeFieldLength(sizeFieldLength)
{
	if(!(sizeFieldLength==1||sizeFieldLength==2||sizeFieldLength==4))
		throw IllegalArgumentException("sizeFieldLength can only be 1 or 2 or 4 bytes");
}

uint FrameInputStream::readFrameSize()
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

boost::shared_ptr<ByteBuffer> FrameInputStream::readFrame()
{
	uint frameSize=readFrameSize();

	return NetUtil::read(this, frameSize);
}

