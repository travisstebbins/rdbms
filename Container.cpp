#include "Container.h"

// constructor (probably not necessary)
template<class T>
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

// constructor for varchars
Container::Container(Type type, varchar value)
{
	_type = type;
	data._varchar = value;
}

// constructor for ints
Container::Container(Type type, int value)
{
	_type = type;
	data._int = value;
}

// copy constructor
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

// returns the stored varchar
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

// returns the stored integer
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

// overloaded copy assignment
Container& Container::operator=(const Container &c)
{
	if(&c == this)
		return *this;
	_type = c.getType();
	if (_type == VARCHAR)
	{
		this->data._varchar = c.getVarchar();
	}
	else
	{
		this->data._int = c.getInt();
	}
	
	return *this;
	// Container newContainer(c.getType(), (c.getType() == VARCHAR) ? c.getVarchar() : c.getInt());
	// return newContainer;
}