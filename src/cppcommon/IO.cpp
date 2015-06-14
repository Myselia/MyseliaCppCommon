#include <cppcommon/IO.h>

using namespace std;
using namespace com::myselia::cppcommon;

//IoService
IoService::IoService()
{
	//Tell the io_service not to stop running until <code>work</code> is destroyed.
	work=new asio::io_service::work(io_service);

	//Start multiple threads as workers to handle io_service async operations.
	for(int i=0; i<NUMBER_OF_WORKER; i++)
		serviceThreads.create_thread(bind(&IoService::serviceHandlerThread, this));
}

IoService::~IoService()
{
	//Tell io_service to stop once all pending operations are done.
	delete work;
	io_service.stop();

	//Wait for all pending operations to be done.
	serviceThreads.join_all();
}

asio::io_service& IoService::getBoostIoService()
{
	return io_service;
}

void IoService::serviceHandlerThread()
{
	io_service.run();
}

//AsioOutputStream
AsioOutputStream::AsioOutputStream(boost::shared_ptr<Socket> socket): socket(socket)
{
	//Do nothing
}

void AsioOutputStream::write(ByteBuffer& buffer)
{
	try
	{
		asio::mutable_buffers_1 asioBuffer(buffer.getData(), buffer.getSize());
		future<size_t> future=async_write(*socket->getAsioSocket(), asioBuffer, boost::asio::use_future);

		//Wait till the operation is done
		future.get();
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

void AsioOutputStream::write(uchar val)
{
	ByteBuffer buffer(1);
	buffer[0]=val;

	write(buffer);
}

//Socket
Socket::Socket(boost::shared_ptr<asio_socket> socket): socket(socket)
{
	//Do nothing
}

Socket::Socket(string host, int port): socket(boost::shared_ptr<asio_socket>(new asio_socket(IoService::getInstance().getBoostIoService())))
{
	try
	{
		asio_endpoint ep(asio::ip::address::from_string(host), port);
		future<void> future=socket->async_connect(ep, boost::asio::use_future);

		//Wait till the operation is done
		future.get();
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

boost::shared_ptr<asio_socket> Socket::getAsioSocket()
{
	return socket;
}

boost::shared_ptr<Socket> Socket::getThisSocket()
{
	return shared_from_this();
}

boost::shared_ptr<InputStream> Socket::getInputStream()
{
	return boost::shared_ptr<InputStream>(new AsioInputStream(getThisSocket()));
}

boost::shared_ptr<OutputStream> Socket::getOutputStream()
{
	return boost::shared_ptr<OutputStream>(new AsioOutputStream(getThisSocket()));
}

//ServerSocket
ServerSocket::ServerSocket(int port)
{
	try
	{
		acceptor=boost::shared_ptr<asio_acceptor>(new asio_acceptor(IoService::getInstance().getBoostIoService(), asio_endpoint(boost::asio::ip::tcp::v4(), port)));
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

boost::shared_ptr<Socket> ServerSocket::accept()
{
	try
	{
		boost::shared_ptr<asio_socket> socket(new asio_socket(IoService::getInstance().getBoostIoService()));

		future<void> future=acceptor->async_accept(*socket, boost::asio::use_future);

		//Wait till the operation is done
		future.get();

		return boost::shared_ptr<Socket>(new Socket(socket));
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

