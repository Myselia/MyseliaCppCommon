#ifndef INCLUDE_CPPCOMMON_JSONTRANSLATOR_H_
#define INCLUDE_CPPCOMMON_JSONTRANSLATOR_H_

#include <cppcommon/Json.h>
#include <common/communication/units/Transmission.h>

using namespace com::myselia::cppcommon;
using namespace com::myselia::common::communication::units;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class JsonTranslator
{
	public:
	static boost::shared_ptr<JsonObject> transmissionToJson(boost::shared_ptr<Transmission> transmission)
	{
		boost::shared_ptr<JsonObject> jsonTransmission(new JsonObject());

		(*jsonTransmission)["header"]=headerToJson(transmission->get_header());

		boost::shared_ptr<JsonArray> jsonAtoms(new JsonArray());
		vector<boost::shared_ptr<Atom>> atoms=transmission->get_atoms();

		for(vector<boost::shared_ptr<Atom>>::iterator it=atoms.begin(); it!=atoms.end(); it++)
			jsonAtoms->add(atomToJson(*it));

		(*jsonTransmission)["atoms"]=jsonAtoms;

		return jsonTransmission;
	}

	static boost::shared_ptr<JsonObject> headerToJson(boost::shared_ptr<Header> header)
	{
		boost::shared_ptr<JsonObject> jsonHeader(new JsonObject());

		(*jsonHeader)["id"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(header->get_id()));
		(*jsonHeader)["from"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(header->get_from()));
		(*jsonHeader)["to"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(header->get_to()));

		return jsonHeader;
	}

	static boost::shared_ptr<JsonObject> atomToJson(boost::shared_ptr<Atom> atom)
	{
		boost::shared_ptr<JsonObject> jsonAtom(new JsonObject());

		(*jsonAtom)["field"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(atom->get_field()));
		(*jsonAtom)["type"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(atom->get_type()));
		(*jsonAtom)["value"]=boost::shared_ptr<JsonPrimitive>(new JsonPrimitive(atom->get_value()));

		return jsonAtom;
	}

	static boost::shared_ptr<Transmission> jsonToTransmission(boost::shared_ptr<JsonObject> jsonObj)
	{
		boost::shared_ptr<Header> header=jsonToHeader(boost::static_pointer_cast<JsonObject>((*jsonObj)["header"]));

		boost::shared_ptr<JsonArray> jsonAtoms=boost::static_pointer_cast<JsonArray>((*jsonObj)["atoms"]);
		vector<boost::shared_ptr<Atom>> atoms;

		for(list<boost::shared_ptr<JsonElement>>::iterator it=jsonAtoms->begin(); it!=jsonAtoms->end(); it++)
			atoms.push_back(jsonToAtom(boost::static_pointer_cast<JsonObject>(*it)));

		boost::shared_ptr<Transmission> transmission(new Transmission());
		transmission->add_header(header);
		transmission->add_atoms(atoms);

		return transmission;
	}

	static boost::shared_ptr<Header> jsonToHeader(boost::shared_ptr<JsonObject> jsonObj)
	{
		int id=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["id"]))->getAsInt();
		string from=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["from"]))->getAsString();
		string to=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["to"]))->getAsString();

		return boost::shared_ptr<Header>(new Header(id, from, to));
	}

	static boost::shared_ptr<Atom> jsonToAtom(boost::shared_ptr<JsonObject> jsonObj)
	{
		string field=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["field"]))->getAsString();
		string type=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["type"]))->getAsString();
		string value=(boost::static_pointer_cast<JsonPrimitive>((*jsonObj)["value"]))->getAsString();

		return boost::shared_ptr<Atom>(new Atom(field, type, value));
	}
};

}
}
}

#endif /* INCLUDE_CPPCOMMON_JSONTRANSLATOR_H_ */
