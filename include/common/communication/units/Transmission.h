#ifndef INCLUDE_COMMON_COMMUNICATION_UNITS_TRANSMISSION_H_
#define INCLUDE_COMMON_COMMUNICATION_UNITS_TRANSMISSION_H_

#include <common/communication/units/Atom.h>
#include <common/communication/units/Header.h>

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

class Transmission
{
	public:
	Transmission()
	{
		//Do nothing
	}

	Transmission(int id, string from, string to) :
			header(new Header(id, from, to))
	{
		//Do nothing
	}

	void add_header(boost::shared_ptr<Header> header)
	{
		this->header=header;
	}

	void add_atoms(vector<boost::shared_ptr<Atom>> list)
	{
		atoms.insert(atoms.end(), list.begin(), list.end());
	}

	boost::shared_ptr<Header> get_header()
	{
		return header;
	}

	vector<boost::shared_ptr<Atom>> get_atoms()
	{
		return atoms;
	}

	bool isValid()
	{
		return header&&atoms.size()>0;
	}

	void printTransmission()
	{
		cout << "id:" << header->get_id() << " ";
		cout << "from:" << header->get_from() << " ";
		cout << "to:" << header->get_to() << " ";
		cout << "atoms:" << atoms.size();
	}

	private:
	boost::shared_ptr<Header> header;
	vector<boost::shared_ptr<Atom>> atoms;
};

}
}
}
}
}

#endif /* INCLUDE_COMMON_COMMUNICATION_UNITS_TRANSMISSION_H_ */
