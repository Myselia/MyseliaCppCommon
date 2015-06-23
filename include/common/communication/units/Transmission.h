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

	Transmission(int id, string from, string to): header(new Header(id, from, to))
	{
		//Do nothing
	}

	Transmission(string to): Transmission(0, "", to)
	{
		//Do nothing
	}

	void setId(int id)
	{
		header->setId(id);
	}

	void setFrom(string from)
	{
		header->setFrom(from);
	}

	void setTo(string to)
	{
		header->setTo(to);
	}

	string getFrom()
	{
		return header->getFrom();
	}

	string getTo()
	{
		return header->getTo();
	}

	int getId()
	{
		return header->getId();
	}

	void setHeader(boost::shared_ptr<Header> header)
	{
		this->header=header;
	}

	void addAtom(boost::shared_ptr<Atom> atom)
	{
		atoms.push_back(atom);
	}

	void addStringAtom(string value)
	{
		addAtom(boost::shared_ptr<Atom>(new Atom(value)));
	}

	void addAtoms(vector<boost::shared_ptr<Atom>> list)
	{
		atoms.insert(atoms.end(), list.begin(), list.end());
	}

	boost::shared_ptr<Header> getHeader()
	{
		return header;
	}

	vector<boost::shared_ptr<Atom>> getAtoms()
	{
		return atoms;
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
