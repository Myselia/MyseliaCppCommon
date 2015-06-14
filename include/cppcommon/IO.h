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

#include <cppcommon/JavaIO.h>

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

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class Socket: public Channel, public boost::enable_shared_from_this<Socket>
{
	public:
	Socket(boost::shared_ptr<asio_socket> socket);
	Socket(const string host, int port);

	boost::shared_ptr<asio_socket> getAsioSocket();
	boost::shared_ptr<InputStream> getInputStream();
	boost::shared_ptr<OutputStream> getOutputStream();

	boost::shared_ptr<Socket> getThisSocket();

	private:
	boost::shared_ptr<asio_socket> socket;
};

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * This class is thread safe.
 */
class AsioInputStream: public InputStream
{
	public:
	AsioInputStream(boost::shared_ptr<Socket> socket): socket(socket), WAIT_TIME(50), operationDone(true), eofReached(false)
	{
		//Do nothing
	}

	~AsioInputStream()
	{
		//Wait until there is no pending read. Otherwise ASOI will not like trying to call a method on a destroyed instance.
		while(!operationDone)
		{
			try
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));
			}
			catch(boost::thread_interrupted& e)
			{
				continue;
			}
		}
	}


	/**
	 * Read from this stream, this will attempt to fill the buffer with data.
	 * This returns the number of bytes read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * throws IOException
	 */
	int read(ByteBuffer& buffer)
	{
		operationMutex.lock();

		//EOF Already reached, this is not changing.
		if(eofReached)
		{
			operationMutex.unlock();
			return -1;
		}

		//If we already have enough unread bytes in our buffer.
		if(this->buffer.getSize()>=buffer.getSize())
		{
			buffer=this->buffer.removeFront(buffer.getSize());
			operationMutex.unlock();
			return buffer.getSize();
		}

		//If there is already a read in progress (the previous attempt to read was interrupted but the data still came back).
		if(!operationDone)
		{
			//Wait for the operation to complete and then retry.
			try
			{
				while(!operationDone)
					boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));
			}
			catch(boost::thread_interrupted& e)
			{
				operationMutex.unlock();
				throw e;
			}

			//Retry
			operationMutex.unlock();
			return read(buffer);
		}

		//Here there is not enough bytes in the buffer and no pending operation. Check that there was not an error at the last read and then start a new read.

		//Check that there was not an error at the last read.
		if(errorReading)
		{
			if(errorReading==asio::error::eof)
			{
				eofReached=true;
				operationMutex.unlock();
				return -1;
			}
			else
			{
				operationMutex.unlock();
				throw IOException(errorReading);
			}
		}

		//Get the bytes we are missing to be able to return the right amount of bytes.
		startRead(buffer.getSize()-this->buffer.getSize());

		//Wait for the operation to complete.
		try
		{
			while(!operationDone)
				boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));
		}
		catch(boost::thread_interrupted& e)
		{
			operationMutex.unlock();
			throw e;
		}

		if(errorReading)
		{
			if(errorReading==asio::error::eof)
			{
				eofReached=true;
				operationMutex.unlock();
				return -1;
			}
			else
			{
				operationMutex.unlock();
				throw IOException(errorReading);
			}
		}
		else
		{
			buffer=this->buffer.removeFront(buffer.getSize());
			operationMutex.unlock();
			return buffer.getSize();
		}
	}

	/**
	 * Read one byte from this stream.
	 * This returns the byte read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * The byte read is returned as an unsigned char for compatibility with Java API.
	 *
	 * throws IOException
	 */
	int read()
	{
		ByteBuffer buffer(1);

		if(read(buffer)==-1)
			return -1;

		uchar val=buffer[0];

		return val;
	}

	private:
	boost::shared_ptr<Socket> socket;
	boost::mutex operationMutex;
	bool operationDone;
	int WAIT_TIME;
	system::error_code errorReading;
	bool eofReached;
	ByteBuffer readPendingBuffer;
	ByteBuffer buffer;

	void startRead(size_t numBytesToRead)
	{
		readPendingBuffer.reset(numBytesToRead);

		operationDone=false;

		asio::mutable_buffers_1 asioBuffer(readPendingBuffer.getData(), readPendingBuffer.getSize());
		async_read(*(socket->getAsioSocket()), asioBuffer,
				boost::bind(&AsioInputStream::readDone, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	}

	void readDone(const system::error_code& error, size_t bytesRead)
	{
		buffer+=readPendingBuffer;
		readPendingBuffer.clear();

		errorReading=error;
		operationDone=true;
	}
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
