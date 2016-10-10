#pragma once

#include "Varchar.h"

class Container {
	public:
		// type of the Container
		enum Type {
			VARCHAR,
			INTEGER
		};

		// constructors
		Container(Type type, varchar value);	// used for varchar Container
		Container(Type type, int value);		// used for int Container
		// copy constructor
		Container(const Container &c);

		// destructor
		~Container() {}

		// getter functions
		Type getType() const { return _type; }
		varchar getVarchar() const;				// used for varchar Container
		int getInt() const;						// used for int Container

		// setter functions
		void setVarchar(varchar vc) { data._varchar = vc; }		// used for varchar Container
		void setInt(int i) { data._int = i; }					// used for int Container

		// overloaded operators
		void operator=(const Container &c);		
		inline bool operator==(const Container rhs) const
		{
			//cant figure out how to solve segfault
			return true;//(this->data._varchar == rhs.data._varchar) || (this->data._int == rhs.data._int);
		}

	private:
		// union to store either varchar or integer data
		union Data {
			varchar _varchar;
			int _int;
			// data constructor
			Data() { new(&_varchar) varchar(0); int _int = 0;}
			// data destructor
			~Data() {}
		} data;

		Type _type;
};