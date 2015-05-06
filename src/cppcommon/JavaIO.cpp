#include "../../include/cppcommon/JavaIO.h"

using namespace com::myselia::cppcommon;

//FilterInputStream
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

//FilterOutputStream
FilterOutputStream::FilterOutputStream(boost::shared_ptr<OutputStream> out): out(out)
{
	//Do nothing
}

void FilterOutputStream::write(ByteBuffer& buffer)
{
	out->write(buffer);
}

void FilterOutputStream::write(uchar val)
{
	out->write(val);
}
