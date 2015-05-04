#include "../../include/cppcommon/JavaIO.h"

using namespace com::myselia::cppcommon;

FilterInputStream::FilterInputStream(boost::shared_ptr<InputStream> in): in(in)
{
	//Do nothing
}

int FilterInputStream::read()
{
	return in->read();
}

int FilterInputStream::read(ByteBuffer& buffer)
{
	return in->read(buffer);
}

