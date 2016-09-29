#include "Varchar.h"

varchar::varchar(int _limit)
	: limit (_limit) {}//{data = "t";}

void varchar::setString(string s)
{
	if (s.length() > limit)
		throw "Varchar exceeded maximum size";
	else
		data = s;
}