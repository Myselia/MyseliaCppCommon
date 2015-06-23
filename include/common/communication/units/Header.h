#ifndef INCLUDE_COMMON_COMMUNICATION_UNITS_HEADER_H_
#define INCLUDE_COMMON_COMMUNICATION_UNITS_HEADER_H_

#include <cppcommon/Generic.h>

using namespace boost;
using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace common
{
namespace communication
{
namespace units
{

class Header
{
	public:
	Header(int id, string from, string to): id(id), from(from), to(to)
	{
		//Do nothing
	}

	void setId(int id)
	{
		this->id=id;
	}

	void setFrom(string from)
	{
		this->from=from;
	}

	void setTo(string to)
	{
		this->to=to;
	}

	int getId()
	{
		return id;
	}

	string getFrom()
	{
		return from;
	}

	string getTo()
	{
		return to;
	}

	private:
	int id;
	string from;
	string to;
};

}
}
}
}
}

#endif /* INCLUDE_COMMON_COMMUNICATION_UNITS_HEADER_H_ */
