#include "../../../include/common/generic/FrameInputStream.h"

/*
using namespace com::myselia::common::generic;

FrameInputStream::FrameInputStream(InputStream fin): FrameInputStream(fin, 4)
{
	//Do nothing
}

FrameInputStream::FrameInputStream(InputStream fin, uint sizeFieldLength): FilterInputStream(fin), sizeFieldLength(sizeFieldLength)
{
	if(!(sizeFieldLength==1||sizeFieldLength==2||sizeFieldLength==4))
		throw IllegalArgumentException("sizeFieldLength can only be 1 or 2 or 4 bytes");
}

uint FrameInputStream::readFrameSize()
{
	if(sizeFieldLength==1)
	{
		NetUtil::read(this, 1);

		return NetUtil.ubyte(NetUtil::read(this, 1)[0]);
	}
	else if(sizeFieldLength==2)
	{
		return ByteBuffer.wrap(NetUtil::read(this, 2)).getShort();
	}
	else
	{
		return ByteBuffer.wrap(NetUtil::read(this, 4)).getInt();
	}
}

asio::mutable_buffers_1 FrameInputStream::readFrame()
{
	//
}
*/
