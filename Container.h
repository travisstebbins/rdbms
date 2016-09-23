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

		void operator=(const Container &c);

	private:
		union Data {
			varchar _varchar;
			int _int;

			Data() { new(&_varchar) varchar(0); }
			~Data() {}
		} data;

		Type _type;

};