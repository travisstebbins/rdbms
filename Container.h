#pragma once

#include "Varchar.h"

class Container {
	public:
		enum Type {
			VARCHAR,
			INTEGER
		};

		template<typename T>
		Container(Type type, T value);
		Container(Type type, varchar value);
		Container(Type type, int value);

		Container(const Container &c);

		~Container() {}

		varchar getVarchar() const;
		int getInt() const;

		Type getType() const { return _type; }

		void setVarchar(varchar vc) { data._varchar = vc; }
		void setInt(int i) { data._int = i; }

		Container& operator=(const Container &c);
		
		// inline bool operator==(const Container rhs) const
		// {
			// return true;//(this->data._varchar == rhs.data._varchar) || (this->data._int == rhs.data._int);
		// }

	private:
		union Data {
			varchar _varchar;
			int _int;

			Data() { new(&_varchar) varchar(0); _int = 0; }
			~Data() {}
		} data;

		Type _type;
		
};