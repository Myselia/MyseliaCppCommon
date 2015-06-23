#ifndef INCLUDE_COMMON_COMMUNICATION_UNITS_ATOM_H_
#define INCLUDE_COMMON_COMMUNICATION_UNITS_ATOM_H_

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

class Atom
{
	public:
	Atom(string field, string type, string value): field(field), type(type), value(value)
	{
		//Do nothing
	}

	Atom(string value): Atom("", "", value)
	{
		//Do nothing
	}

	string get_field()
	{
		return field;
	}

	string get_type()
	{
		return type;
	}

	string get_value()
	{
		return value;
	}

	private:
	string field;
	string type;
	string value;
};

}
}
}
}
}

#endif /* INCLUDE_COMMON_COMMUNICATION_UNITS_ATOM_H_ */
