/**
 * Generic definitions and classes that can be used everywhere.
 */

#ifndef INCLUDE_GENERIC_H_
#define INCLUDE_GENERIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <exception>

#include <boost/shared_ptr.hpp>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

using namespace std;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class GenericException: public runtime_error
{
	public:
	GenericException(): runtime_error("")
	{
		message="";
	}

	GenericException(string message): runtime_error(message)
	{
		this->message=message;
	}

	GenericException(exception cause): runtime_error("")
	{
		message="";
		this->cause=cause;
	}

	GenericException(exception cause, string message): runtime_error(message)
	{
		this->message=message;
		this->cause=cause;
	}

	string getMessage()
	{
		return message;
	}

	exception getCause()
	{
		return cause;
	}

	const char* what() const throw()
	{
		return message.c_str();
	}

	private:
	string message;
	exception cause;
};

class IllegalStateException: public GenericException
{
	public:
	IllegalStateException()
	{
		//Do nothing
	}

	IllegalStateException(string message): GenericException(message)
	{
		//Do nothing
	}

	IllegalStateException(exception cause): GenericException(cause)
	{
		//Do nothing
	}
};

class IllegalArgumentException: public GenericException
{
	public:
	IllegalArgumentException()
	{
		//Do nothing
	}

	IllegalArgumentException(string message): GenericException(message)
	{
		//Do nothing
	}

	IllegalArgumentException(exception cause): GenericException(cause)
	{
		//Do nothing
	}
};

class ByteBuffer
{
	public:
	ByteBuffer(): size(0), data(NULL)
	{
		//Do nothing
	}

	/**
	 * Creates a buffer of length <code>size</code> (not initialized).
	 */
	ByteBuffer(size_t size)
	{
		reset(size);
	}

	/**
	 * Creates a copy of the provided buffer.
	 */
	ByteBuffer(const ByteBuffer& buffer): data(NULL)
	{
		copy(buffer);
	}

	/**
	 * Creates a buffer with the same length and value as the string.
	 */
	ByteBuffer(string str): data(NULL)
	{
		copy(str.c_str(), str.length());
	}

	~ByteBuffer()
	{
		clear();
	}

	void* getData() const
	{
		return data;
	}

	size_t getSize() const
	{
		return size;
	}

	const string getAsString() const
	{
		return string((char*)data);
	}

	void clear()
	{
		if(data)
		{
			delete data;
			data=NULL;
			size=0;
		}
	}

	void reset(size_t size)
	{
		if(size==0)
			clear();
		else
		{
			this->size=size;
			data=new uchar[size];
		}
	}

	void copy(const void* bufData, const size_t& bufSize)
	{
		const uchar* bufChardata=(const uchar*)bufData;

		clear();

		size=bufSize;
		data=new uchar[size];

		for(size_t i=0; i<size; i++)
			data[i]=bufChardata[i];
	}

	void copy(const ByteBuffer& buffer)
	{
		copy(buffer.getData(), buffer.getSize());
	}

	/**
	 * Returns a buffer that contains the first <code>sizeToRemove</code> bytes of this buffer;
	 * also remove those bytes from this buffer.
	 */
	ByteBuffer removeFront(size_t sizeToRemove)
	{
		if(sizeToRemove>size)
			throw IllegalArgumentException("Not enough bytes: "+to_string(sizeToRemove)+" > "+to_string(size));

		ByteBuffer frontBuf(sizeToRemove);

		for(size_t i=0; i<sizeToRemove; i++)
			frontBuf[i]=data[i];

		if(sizeToRemove==size)
			clear();
		else
		{
			uchar* tmpData=new uchar[size-sizeToRemove];

			for(size_t i=sizeToRemove; i<size; i++)
				tmpData[i-sizeToRemove]=data[i];

			delete data;
			data=tmpData;
			size-=sizeToRemove;
		}

		return frontBuf;
	}

	uchar& operator[](size_t position) const
	{
		if(position>size-1)
			throw IllegalArgumentException("Position out of bound: "+to_string(position));

		return data[position];
	}

	void operator+=(const ByteBuffer& buffer)
	{
		uchar tmpBuf[size];

		for(size_t i=0; i<size; i++)
			tmpBuf[i]=data[i];

		const size_t oldSize=size;

		delete data;
		size+=buffer.getSize();
		data=new uchar[size];

		for(size_t i=0; i<oldSize; i++)
			data[i]=tmpBuf[i];

		for(size_t i=oldSize; i<size; i++)
			data[i]=buffer[i-oldSize];
	}

	void operator=(const ByteBuffer& buffer)
	{
		copy(buffer);
	}

	friend ostream& operator<<(ostream& os, const ByteBuffer& buffer)
	{
		os << buffer.getAsString();

		return os;
	}

	private:
	uchar* data;
	size_t size;
};

class GenericUtil
{
	public:
	static char getSecondLastDigit(int num)
	{
		string str=to_string(num);

		if(str.length()==1)
			return str[0];

		return str[str.length()-2];
	}

	static void printScale(int len)
	{
		for(int i=0; i<len; i++)
		{
			if(i%10==0)
				cout << getSecondLastDigit(i);
			else if(i%5==0)
				cout << "+";
			else
				cout << "-";
		}

		cout << endl;
	}

	static void tokenize(const string& str, vector<string>& tokens, const char delimiter = ' ')
	{
	    string token="";

	    for(size_t i=0; i<str.length(); i++)
	    {
	    	if(str[i]==delimiter)
	    	{
	    		tokens.push_back(token);
	    		token="";
	    	}
	    	else
	    		token+=str[i];
	    }

	    tokens.push_back(token);
	}
};

}
}
}

#endif /* INCLUDE_GENERIC_H_ */
