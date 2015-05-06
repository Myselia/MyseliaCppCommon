#include "../../include/cppcommon/Generic.h"
#include "../../include/cppcommon/Exception.h"

using namespace std;
using namespace com::myselia::cppcommon;

//ByteBuffer
ByteBuffer::ByteBuffer(size_t size): size(size)
{
	data=new uchar[size];
}

ByteBuffer::ByteBuffer(const ByteBuffer& buffer): size(buffer.size)
{
	data=new uchar[size];

	for(size_t i=0; i<size; i++)
		data[i]=buffer.data[i];
}

ByteBuffer::ByteBuffer(string str): ByteBuffer(str.length())
{
	for(size_t i=0; i<size; i++)
		data[i]=str[i];
}

ByteBuffer::~ByteBuffer()
{
	delete data;
}

void* ByteBuffer::getData()
{
	return data;
}

size_t ByteBuffer::getSize()
{
	return size;
}

uchar& ByteBuffer::operator[](size_t position)
{
	if(position>size-1)
		throw IllegalArgumentException("Position out of bound: "+to_string(position));

	return data[position];
}

//GenericUtil
char GenericUtil::getSecondLastDigit(int num)
{
	string str=to_string(num);

	if(str.length()==1)
		return str[0];

	return str[str.length()-2];
}

void GenericUtil::printScale(int len)
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
