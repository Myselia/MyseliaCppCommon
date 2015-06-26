#ifndef INCLUDE_JSON_H_
#define INCLUDE_JSON_H_

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <cppcommon/Generic.h>

using namespace std;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class JsonParseException: public GenericException
{
	public:
	JsonParseException(): str(""), position(0)
	{
		//Do nothing
	}

	JsonParseException(string message): GenericException(message), str(""), position(0)
	{
		//Do nothing
	}

	JsonParseException(string message, string str, int position): GenericException(message), str(str), position(position)
	{
		//Do nothing
	}

	JsonParseException(exception cause): GenericException(cause), str(""), position(0)
	{
		//Do nothing
	}

	virtual string getMessage() const
	{
		string message="";
		message+=GenericException::getMessage();

		if(str!="")
		{
			message+="\r\n"+str+"\r\n";
			message+=GenericUtil::repeat(" ", position);
			message+="^";
		}

		return message;
	}

	private:
	string str;
	int position;
};

class JsonElement;

class JsonElement: public boost::enable_shared_from_this<JsonElement>
{
	public:
	enum JsonElementType
	{
		JsonNull, JsonPrimitive, JsonArray, JsonObject
	};

	bool isJsonNull();
	bool isJsonPrimitive();
	bool isJsonArray();
	bool isJsonObject();

	JsonElementType getJsonElementType();
	string toJsonString();

	protected:
	JsonElement(JsonElementType type);

	private:
	JsonElementType type;
};

class JsonNull: public JsonElement
{
	public:
	JsonNull();
};

class JsonPrimitive: public JsonElement
{
	public:
	enum PrimitiveType
	{
		Bool, Int, String
	};

	JsonPrimitive(bool value);
	JsonPrimitive(int value);
	JsonPrimitive(string value);
	JsonPrimitive(const char* value);
	~JsonPrimitive();

	bool getAsBool() const;
	int getAsInt() const;
	string getAsString() const;

	PrimitiveType getPrimitiveType() const;
	bool isPrimitiveType(PrimitiveType primitiveType) const;

	bool isBool() const;
	bool isInt() const;
	bool isString() const;

	private:
	boost::variant<bool, int, string> val;
};

class JsonArray: public JsonElement
{
	public:
	JsonArray();

	uint size();
	void add(const boost::shared_ptr<JsonElement>& element);

	list<boost::shared_ptr<JsonElement>>::iterator begin();
	list<boost::shared_ptr<JsonElement>>::iterator end();

	private:
	list<boost::shared_ptr<JsonElement>> elements;
};

class JsonObject: public JsonElement
{
	public:
	JsonObject();

	boost::shared_ptr<JsonElement>& operator[](string name);

	std::unordered_map<string, boost::shared_ptr<JsonElement>>::iterator begin();
	std::unordered_map<string, boost::shared_ptr<JsonElement>>::iterator end();

	private:
	std::unordered_map<string, boost::shared_ptr<JsonElement>> map;
};

/**
 * Json serializer and deserializer.
 */
class Json
{
	public:
	static boost::shared_ptr<JsonElement> parse(string jsonString);
	static string serialize(const boost::shared_ptr<JsonElement>& element);

	private:
	static boost::shared_ptr<JsonElement> parse(string jsonString, int& position);
	static boost::shared_ptr<JsonObject> parseObject(string jsonString, int& position);
	static boost::shared_ptr<JsonArray> parseArray(string jsonString, int& position);
	static boost::shared_ptr<JsonPrimitive> parsePrimitive(string jsonString, int& position);
	static boost::shared_ptr<JsonNull> parseNull(string jsonString, int& position);
	static void skipSpaces(string jsonString, int& position);
	static char read(string jsonString, int position);
	static bool checkEqual(string jsonString, int position, string needle);

	/**
	 * Parse a C string without the delimiters.
	 * This method keeps reading until it hits stopChar.
	 */
	static string parseCString(string jsonString, int& position, char stopChar);
	static string escapeSpecialStringCharacters(string str);

	static string serializeObject(const boost::shared_ptr<JsonObject>& object);
	static string serializeArray(const boost::shared_ptr<JsonArray>& array);
	static string serializePrimitive(const boost::shared_ptr<JsonPrimitive>& primitive);
	static string serializeNull(const boost::shared_ptr<JsonNull>& null);
};

}
}
}

#endif /* INCLUDE_JSON_H_ */
