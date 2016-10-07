#pragma once

#include <string>

using namespace std;

class varchar
{
	string data;
	int limit;

	public:
		varchar(int _limit);
		~varchar() {}
		string getString() { return data; }
		void setString(string s);
		string getData() const { return data;}
		int getLimit() const {return limit;}
		// varchar& operator=(const varchar& other)
		// {
			// if(&other == this)
				// return *this;
			// this->data = other.getData();
			// this->limit = other.getLimit();
			// return *this;
		// }
		inline bool operator==(const varchar& rhs) const
		{
			return (this->data == rhs.data);
		}
};