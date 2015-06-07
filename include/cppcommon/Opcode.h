#ifndef INCLUDE_CPPCOMMON_OPCODE_H_
#define INCLUDE_CPPCOMMON_OPCODE_H_

#include <cppcommon/Exception.h>

using namespace std;
using namespace boost;
using namespace com::myselia::common::communication::units;

namespace com
{
namespace myselia
{
namespace cppcommon
{

class ComponentType
{
	public:
	static const ComponentType STEM=ComponentType("2");
	static const ComponentType DAEMON=ComponentType("3");
	static const ComponentType SANDBOX_MASTER=ComponentType("4");
	static const ComponentType SANDBOX_SLAVE=ComponentType("5");

	string getValue() const
	{
		return value;
	}

	bool operator==(ComponentType componentType) const
	{
		return getValue()==componentType.getValue();
	}

	private:
	string value;

	ComponentType(string value): value(value) {};
};

class ActionType
{
	public:
	static const ActionType SETUP=ActionType("0");
	static const ActionType RUNTIME=ActionType("2");
	static const ActionType ERROR=ActionType("4");
	static const ActionType DATA=ActionType("6");

	string getValue() const
	{
		return value;
	}

	bool operator==(ActionType actionType) const
	{
		return getValue()==actionType.getValue();
	}

	private:
	string value;

	ActionType(string value): value(value) {};
};

class Opcode
{
	public:
	Opcode(ComponentType componentType, ActionType actionType, string operation): componentType(componentType), actionType(actionType), operation(operation) {};

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
		return componentType.getValue()+actionType.getValue()+operation;
	}

	private:
	ComponentType componentType;
	ActionType actionType;
	string operation;
};

}
}
}


#endif /* INCLUDE_CPPCOMMON_OPCODE_H_ */
