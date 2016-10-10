#include "Varchar.h"

// constructor
varchar::varchar(int _limit)
	: limit (_limit), data("") {}//{data = "t";}

// sets the data to a new string value
void varchar::setString(string s)
{
	if (s.length() > limit)
		throw "Varchar exceeded maximum size";
	else
		data = s;
}