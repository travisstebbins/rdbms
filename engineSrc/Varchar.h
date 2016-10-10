#pragma once

#include <string>

using namespace std;

class varchar
{
	// string to hold
	string data;
	// character limit
	int limit;

	public:
		// constructor
		varchar(int _limit);
		// destructor
		~varchar() {}

		// getter functions
		string getString() { return data; }
		string getData() const { return data;}
		int getLimit() const {return limit;}

		// setter functions
		void setString(string s);

		// overloaded comparison operator
		inline bool operator==(const varchar& rhs) const
		{
			return (this->data == rhs.data);
		}
};