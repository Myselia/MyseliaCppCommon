#ifndef INCLUDE_CPPCOMMON_JAVAIO_H_
#define INCLUDE_CPPCOMMON_JAVAIO_H_

/**
 * Classes from java.io.* package that can be implemented in C++ without heavily depending on an external library.
 */

#include "../../include/cppcommon/Exception.h"

using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class InputStream
{
	public:
	virtual ~InputStream() {};

	/**
	 * Read from this stream, this will attempt to fill the buffer with data.
	 * This returns the number of bytes read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * throws IOException
	 */
	virtual int read(ByteBuffer& buffer) = 0;

	/**
	 * Read one byte from this stream.
	 * This returns the byte read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * The byte read is returned as an unsigned char for compatibility with Java API.
	 *
	 * throws IOException
	 */
	virtual int read() = 0;
};

class OutputStream
{
	public:
	virtual ~OutputStream() {};

	/**
	 * Write then entire buffer to this output stream.
	 *
	 * throws IOException
	 */
	virtual void write(ByteBuffer& buffer) = 0;

	/**
	 * Write one byte to this output stream.
	 *
	 * throws IOException
	 */
	virtual void write(uchar val) = 0;
};

class FilterInputStream: public InputStream
{
	public:
	FilterInputStream(boost::shared_ptr<InputStream> in);

	int read();
	int read(ByteBuffer& buffer);

	private:
	boost::shared_ptr<InputStream> in;
};

class FilterOutputStream: public OutputStream
{
	public:
	FilterOutputStream(boost::shared_ptr<OutputStream> out);

	void write(ByteBuffer& buffer);
	void write(uchar val);

	private:
	boost::shared_ptr<OutputStream> out;
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_JAVAIO_H_ */
