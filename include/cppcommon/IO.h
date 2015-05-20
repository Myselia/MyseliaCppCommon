#ifndef INCLUDE_CPPCOMMON_IO_H_
#define INCLUDE_CPPCOMMON_IO_H_

/* IO.h : classes for Network IO.
 *
 * The following classes are wrapper around boost ASIO to expose a similar interface as the java library.
 * This is to allow easier translation of the MyseliaCommon to cpp.
 */

#include <future>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/thread.hpp>

#include "../../include/cppcommon/JavaIO.h"

using namespace boost;

typedef boost::asio::ip::tcp::socket asio_socket;
typedef boost::asio::ip::tcp::acceptor asio_acceptor;
typedef boost::asio::ip::tcp::endpoint asio_endpoint;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class IOException: public GenericException
{
	public:
	IOException()
	{
		//Do nothing
	}

	IOException(string message): GenericException(message)
	{
		//Do nothing
	}

	IOException(exception cause): GenericException(cause)
	{
		//Do nothing
	}

	IOException(system::error_code error): GenericException(error.message())
	{
		//Do nothing
	}
};

class IoService
{
	public:
	static IoService& getInstance()
	{
		static IoService instance;

		return instance;
	}

	asio::io_service& getBoostIoService();

	private:
	int const NUMBER_OF_WORKER=2;
	asio::io_service io_service;
	asio::io_service::work* work;
	thread_group serviceThreads;

	IoService();
	~IoService();

	IoService(IoService const&); //Dont implement (for singleton)
	void operator=(IoService const&); //Dont implement (for singleton)

	void serviceHandlerThread();
};

class Socket;

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class AsioInputStream: public InputStream
{
	public:
	AsioInputStream(boost::shared_ptr<Socket> socket);

	/**
	 * Read from this stream, this will attempt to fill the buffer with data.
	 * This returns the number of bytes read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * throws IOException
	 */
	int read(ByteBuffer& buffer);

	/**
	 * Read one byte from this stream.
	 * This returns the byte read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * The byte read is returned as an unsigned char for compatibility with Java API.
	 *
	 * throws IOException
	 */
	int read();

	private:
	boost::shared_ptr<Socket> socket;
};

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class AsioOutputStream: public OutputStream
{
	public:
	AsioOutputStream(boost::shared_ptr<Socket> socket);

	/**
	 * Write then entire buffer to this output stream.
	 *
	 * throws IOException
	 */
	void write(ByteBuffer& buffer);

	/**
	 * Write one byte to this output stream.
	 *
	 * throws IOException
	 */
	void write(uchar val);

	private:
	boost::shared_ptr<Socket> socket;
};

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class Socket: public boost::enable_shared_from_this<Socket>
{
	public:
	Socket(boost::shared_ptr<asio_socket> socket);
	Socket(const string host, int port);

	boost::shared_ptr<asio_socket> getAsioSocket();
	boost::shared_ptr<InputStream> getInputStream();
	boost::shared_ptr<OutputStream> getOutputStream();

	boost::shared_ptr<Socket> getSocket();

	private:
	boost::shared_ptr<asio_socket> socket;
};

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class ServerSocket
{
	public:
	ServerSocket(int port);

	boost::shared_ptr<Socket> accept();

	private:
	boost::shared_ptr<asio_acceptor> acceptor;
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_IO_H_ */
