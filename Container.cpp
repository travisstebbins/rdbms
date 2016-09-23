#include "Container.h"

template<typename T>
Container::Container(Type type, T value)
{
	_type = type;
	if (type == VARCHAR)
	{
		data._varchar = value;
	}
	else
	{
		data._int = value;
	}
}

Container::Container(const Container &c)
{
	_type = c.getType();
	if (_type == VARCHAR)
	{
		data._varchar = c.getVarchar();
	}
	else
	{
		data._int = c.getInt();
	}
}

varchar Container::getVarchar() const
{
	if (_type == VARCHAR)
	{
		return data._varchar;
	}
	else
	{
		throw "Tried to get an int from a container holding a varchar";
	}
}

int Container::getInt() const
{
	if (_type == INTEGER)
	{
		return data._int;
	}
	else
	{
		throw "Tried to get a varchar from a container holding an int";
	}
}

void Container::operator=(const Container &c)
{
	_type = c.getType();
	if (_type == VARCHAR)
	{
		data._varchar = c.getVarchar();
	}
	else
	{
		data._int = c.getInt();
	}
	// Container newContainer(c.getType(), (c.getType() == VARCHAR) ? c.getVarchar() : c.getInt());
	// return newContainer;
}