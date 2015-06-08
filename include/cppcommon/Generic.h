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

class ByteBuffer
{
	public:
	/**
	 * Creates a buffer of length <code>size</code>.
	 */
	ByteBuffer(size_t size);

	/**
	 * Creates a copy of the provided buffer.
	 */
	ByteBuffer(const ByteBuffer& buffer);

	/**
	 * Creates a buffer with the same length and value as the string.
	 */
	ByteBuffer(string str);

	~ByteBuffer();

	void* getData();
	size_t getSize();

	uchar& operator[](size_t position);

	string getAsString();

	private:
	uchar* data;
	size_t size;

	/**
	 * Disable buffer assignment for now.
	 */
	ByteBuffer& operator=(const ByteBuffer&);
};

class GenericUtil
{
	public:
	static char getSecondLastDigit(int num);
	static void printScale(int len);

	static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
	{
	    // Skip delimiters at beginning.
	    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	    // Find first "non-delimiter".
	    string::size_type pos     = str.find_first_of(delimiters, lastPos);

	    while (string::npos != pos || string::npos != lastPos)
	    {
	        // Found a token, add it to the vector.
	        tokens.push_back(str.substr(lastPos, pos - lastPos));
	        // Skip delimiters.  Note the "not_of"
	        lastPos = str.find_first_not_of(delimiters, pos);
	        // Find next "non-delimiter"
	        pos = str.find_first_of(delimiters, lastPos);
	    }
	}
};

}
}
}

#endif /* INCLUDE_GENERIC_H_ */
