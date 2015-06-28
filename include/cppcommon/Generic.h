/**
 * Generic definitions and classes that can be used everywhere.
 */

#ifndef INCLUDE_GENERIC_H_
#define INCLUDE_GENERIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <exception>
#include <limits>

#include <boost/smart_ptr.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/uuid/sha1.hpp>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef uint uint_array_5[5];

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

	virtual string getMessage() const
	{
		return message;
	}

	exception getCause() const
	{
		return cause;
	}

	const char* what() const throw()
	{
		return getMessage().c_str();
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
	ByteBuffer(size_t size): size(0), data(NULL)
	{
		reset(size);
	}

	/**
	 * Creates a copy of the provided buffer.
	 */
	ByteBuffer(const ByteBuffer& buffer): size(0), data(NULL)
	{
		copy(buffer);
	}

	/**
	 * Creates a buffer with the same length and value as the string.
	 */
	ByteBuffer(string str): size(0), data(NULL)
	{
		copy(str.c_str(), str.length());
	}

	/**
	 * Creates a buffer with length <code>length</code> and same value as the C string.
	 */
	ByteBuffer(char const* cstr, size_t length): size(0), data(NULL)
	{
		copy(cstr, length);
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

	char const* getAsCstring() const
	{
		return (char*)data;
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
		clear();

		if(size>0)
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

class Sha1Hash
{
	public:
	uint_array_5 hash;

	Sha1Hash(uint_array_5& hash)
	{
		for(size_t i=0; i<5; i++)
			this->hash[i]=hash[i];
	}

	size_t getSizeInBytes()
	{
		return sizeof(hash);
	}

	/**
	 * Returns a reference to a uint_array_5 (uint[5]).
	 */
	/*(&operator uint())[5]
	{
		return hash;
	}*/

	operator char*()
	{
		const char* tmp = reinterpret_cast<char*>(hash);

		for(int i = 0; i < 5; ++i)
		{
			bytearr[i*4] = tmp[i*4+3];
			bytearr[i*4+1] = tmp[i*4+2];
			bytearr[i*4+2] = tmp[i*4+1];
			bytearr[i*4+3] = tmp[i*4];
		}

		return bytearr;
	}

	private:
	char bytearr[20]; //For conversion to char*
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

	static string repeat(string str, uint times)
	{
		string ret="";

		for(uint i=0; i<times; i++)
			ret+=str;

		return ret;
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

	static string executeCommand(string command)
	{
		FILE* pipe=popen(command.c_str(), "r");

		if(!pipe)
			return "ERROR";

		char buffer[128];
		std::string result="";

		while(!feof(pipe))
			if(fgets(buffer, 128, pipe)!=NULL)
				result+=buffer;

		pclose(pipe);
		return result;
	}

	static int generateRandomNumber(int min, int max)
	{
		if(min>max)
			throw IllegalArgumentException("min is greater than max.");

		boost::mt19937 generator;
		generator.seed(time(NULL));
		boost::uniform_int<> range(min, max);
		boost::variate_generator<boost::mt19937, boost::uniform_int<>> rangeGenerator(generator, range);

		return rangeGenerator();
	}

	static int generateRandomInt()
	{
		return generateRandomNumber(numeric_limits<int>::min(), numeric_limits<int>::max());
	}

	static int generateRandomPositiveInt()
	{
		return generateRandomNumber(0, numeric_limits<int>::max());
	}

	static uint generateRandomUint()
	{
		return (uint)generateRandomNumber(numeric_limits<uint>::min(), numeric_limits<uint>::max());
	}

	static std::vector<char> readAllBytes(string filename)
	{
		ifstream ifs(filename, ios::binary|ios::ate);
		ifstream::pos_type pos = ifs.tellg();

		std::vector<char>  result(pos);

		ifs.seekg(0, ios::beg);
		ifs.read(&result[0], pos);

		return result;
	}

	static Sha1Hash getSha1Hash(string filename)
	{
		std::ifstream ifs(filename, std::ios::binary);

		if(!ifs.good())
			throw GenericException("Unable to read file: '"+filename+"'");

		boost::uuids::detail::sha1 sha1;
		uint_array_5 mydigest;
		char buf[1024];

		while(ifs.good()) {
			ifs.read(buf, sizeof(buf));
			sha1.process_bytes(buf, ifs.gcount());
		}

		if(!ifs.eof())
			throw GenericException("Error while reading file: '"+filename+"'");

		ifs.close();
		sha1.get_digest(mydigest);

		return Sha1Hash(mydigest);
	}

	static string getHexString(char const* data, size_t size)
	{
		string str;
		char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

		for(size_t i=0; i<size; i++)
		{
		    char const byte = data[i];

		    str+=hex_chars[(byte & 0xF0) >> 4];
		    str+=hex_chars[(byte & 0x0F) >> 0];
		}

		return str;
	}
};

}
}
}

#endif /* INCLUDE_GENERIC_H_ */
