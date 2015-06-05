#ifndef INCLUDE_CPPCOMMON_MESSAGESERVICE_H_
#define INCLUDE_CPPCOMMON_MESSAGESERVICE_H_

#include <cppcommon/Exception.h>

using namespace boost;

namespace com
{
namespace myselia
{
namespace cppcommon
{

//API

class Destination
{
	public:
	virtual ~Destination() {};

	virtual string getDestinationType() const = 0;
};

class Queue: public Destination
{
	public:
	virtual ~Queue() {};

	virtual bool operator==(Queue& queue) = 0;
};

class Topic: public Destination
{
	public:
	virtual ~Topic() {};

	virtual bool operator==(Topic& topic) = 0;
};

class Message
{
	public:
	virtual ~Message() {};
};

typedef boost::function<void(boost::shared_ptr<Message>)> MessageListener;

class MessageConsumer
{
	public:
	virtual ~MessageConsumer() {};

	boost::shared_ptr<MessageListener> getMessageListener()
	{
		return listener;
	}

	void setMessageListener(boost::shared_ptr<MessageListener> messageListener)
	{
		listener=messageListener;
	}

	boost::shared_ptr<Message> receive()
	{
		return receive(-1);
	}

	boost::shared_ptr<Message> receiveNoWait()
	{
		return receive(0);
	}

	/**
	 * Receives a Message.
	 *
	 * @param: timeout
	 * 			Time to wait in miliseconds. If -1 wait forever, if 0 only return a Message right away if there is one.
	 */
	virtual boost::shared_ptr<Message> receive(long timeout) = 0;

	private:
	boost::shared_ptr<MessageListener> listener;
};

class QueueReceiver: public MessageConsumer
{
	public:
	virtual ~QueueReceiver() {};

	virtual boost::shared_ptr<Queue> getQueue() = 0;
};

class TopicSubscriber: public MessageConsumer
{
	public:
	virtual ~TopicSubscriber() {};

	virtual boost::shared_ptr<Topic> getTopic() = 0;
};

class MessageProducer
{
	public:
	virtual ~MessageProducer() {};

	virtual void send(boost::shared_ptr<Destination> destination, boost::shared_ptr<Message> message) = 0;
};

class TopicPublisher: public MessageProducer
{
	public:
	virtual ~TopicPublisher() {};

	virtual void publish(boost::shared_ptr<Topic> topic, boost::shared_ptr<Message> message) = 0;
};

class QueueSender: public MessageProducer
{
	public:
	virtual ~QueueSender() {};

	virtual void send(boost::shared_ptr<Queue> queue, boost::shared_ptr<Message> message) = 0;
};

class Session
{
	public:
	virtual ~Session() {};

	virtual boost::shared_ptr<QueueSender> createQueueSender() = 0;
	virtual boost::shared_ptr<QueueReceiver> createQueueReceiver() = 0;

	virtual boost::shared_ptr<TopicPublisher> createTopicPublisher() = 0;
	virtual boost::shared_ptr<TopicSubscriber> createTopicSubscriber() = 0;
};

//Default implementation

class DefaultQueue: public Queue
{
	public:
	DefaultQueue(string name): name(name) {};

	virtual string getDestinationType() const
	{
		return "DefaultQueue";
	}

	virtual bool operator==(Queue& queue)
	{
		if(getDestinationType()!=queue.getDestinationType())
			return false;

		DefaultQueue& defaultQueue=dynamic_cast<DefaultQueue&>(queue);

		return getQueueName()==defaultQueue.getQueueName();
	}

	virtual string getQueueName()
	{
		return name;
	}

	private:
	string name;
};

class DefaultTopic: public Topic
{
	public:
	DefaultTopic(string name): name(name) {};

	virtual string getDestinationType() const
	{
		return "DefaultTopic";
	}

	virtual  bool operator==(Topic& topic)
	{
		if(getDestinationType()!=topic.getDestinationType())
			return false;

		DefaultTopic& defaultTopic=dynamic_cast<DefaultTopic&>(topic);

		return getTopicName()==defaultTopic.getTopicName();
	}

	virtual string getTopicName()
	{
		return name;
	}

	private:
	string name;
};

class DefaultMessage: public Message
{
	public:
	DefaultMessage() {};
	DefaultMessage(boost::shared_ptr<ByteBuffer> data): data(data) {};

	virtual boost::shared_ptr<ByteBuffer> getMessageData()
	{
		return data;
	}

	private:
	boost::shared_ptr<ByteBuffer> data;
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_MESSAGESERVICE_H_ */
