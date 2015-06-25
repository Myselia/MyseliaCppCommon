#ifndef INCLUDE_CPPCOMMON_OPCODE_H_
#define INCLUDE_CPPCOMMON_OPCODE_H_

#include <cppcommon/Generic.h>

using namespace std;
using namespace boost;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class ComponentType
{
	public:
	static ComponentType STEM;
	static ComponentType DAEMON;
	static ComponentType SANDBOX_MASTER;
	static ComponentType SANDBOX_SLAVE;

	static ComponentType fromString(string& str)
	{
		if(str==STEM.getValue())
			return STEM;
		else if(str==DAEMON.getValue())
			return DAEMON;
		else if(str==SANDBOX_MASTER.getValue())
			return SANDBOX_MASTER;
		else if(str==SANDBOX_SLAVE.getValue())
			return SANDBOX_SLAVE;
		else
			throw IllegalArgumentException("ComponentType.fromString(): Invalid string: \""+str+"\"");
	}

	string getValue() const
	{
		return value;
	}

	bool operator==(const ComponentType& componentType) const
	{
		return getValue()==componentType.getValue();
	}

	private:
	string value;

	ComponentType(string value): value(value) {};
};

ComponentType ComponentType::STEM=ComponentType("2");
ComponentType ComponentType::DAEMON=ComponentType("3");
ComponentType ComponentType::SANDBOX_MASTER=ComponentType("4");
ComponentType ComponentType::SANDBOX_SLAVE=ComponentType("5");

class ActionType
{
	public:
	static ActionType SETUP;
	static ActionType RUNTIME;
	static ActionType ERROR;
	static ActionType DATA;

	static ActionType fromString(string& str)
	{
		if(str==SETUP.getValue())
			return SETUP;
		else if(str==RUNTIME.getValue())
			return RUNTIME;
		else if(str==ERROR.getValue())
			return ERROR;
		else if(str==DATA.getValue())
			return DATA;
		else
			throw IllegalArgumentException("ActionType.fromString(): Invalid string: \""+str+"\"");
	}

	string getValue() const
	{
		return value;
	}

	bool operator==(const ActionType& actionType) const
	{
		return getValue()==actionType.getValue();
	}

	private:
	string value;

	ActionType(string value): value(value) {};
};

ActionType ActionType::SETUP=ActionType("0");
ActionType ActionType::RUNTIME=ActionType("2");
ActionType ActionType::ERROR=ActionType("4");
ActionType ActionType::DATA=ActionType("6");

class Opcode
{
	public:
	Opcode(ComponentType componentType, ActionType actionType, string operation): componentType(componentType), actionType(actionType), operation(operation)
	{
		//Do nothing
	};

	static Opcode fromString(string str)
	{
		vector<string> tokens;

		GenericUtil::tokenize(str, tokens, delimiter);

		if(tokens.size()!=3)
			throw IllegalArgumentException("Opcode.fromString(): Invalid string: \""+str+"\" (tokens.size(): "+to_string(tokens.size())+")");

		return Opcode(ComponentType::fromString(tokens[0]), ActionType::fromString(tokens[1]), tokens[2]);
	}

	ComponentType getComponentType() const
	{
		return componentType;
	}

	ActionType getActionType() const
	{
		return actionType;
	}

	string getOperation() const
	{
		return operation;
	}

	string toString() const
	{
		return componentType.getValue()+delimiter+actionType.getValue()+delimiter+operation;
	}

	bool operator==(const Opcode& opcode) const
	{
		return componentType==opcode.getComponentType()&&actionType==opcode.getActionType()&&operation==opcode.getOperation();
	}

	private:
	static char delimiter;

	ComponentType componentType;
	ActionType actionType;
	string operation;
};

char Opcode::delimiter='_';

class Destination
{
	public:
	Destination(string componentId, Opcode opcode): componentId(componentId), opcode(opcode) {};

	static Destination fromString(string str)
	{
		vector<string> tokens;

		GenericUtil::tokenize(str, tokens, delimiter);

		if(tokens.size()!=2)
			throw IllegalArgumentException("Destination.fromString(): Invalid string: \""+str+"\" (tokens.size(): "+to_string(tokens.size())+")");

		return Destination(tokens[0], Opcode::fromString(tokens[1]));
	}

	void setComponentId(string componentId)
	{
		this->componentId=componentId;
	}

	string getComponentId() const
	{
		return componentId;
	}

	Opcode getOpcode() const
	{
		return opcode;
	}

	string toString() const
	{
		return componentId+delimiter+opcode.toString();
	}

	private:
	static char delimiter;

	string componentId;
	Opcode opcode;
};

char Destination::delimiter=':';

}
}
}

namespace std
{
	template <>
	struct hash<com::myselia::cppcommon::Opcode>
	{
		size_t operator()(const com::myselia::cppcommon::Opcode& opcode) const
		{
			return hash<string>()(opcode.toString());
		}
	};
}

#endif /* INCLUDE_CPPCOMMON_OPCODE_H_ */
