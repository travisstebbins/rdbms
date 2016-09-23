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
};