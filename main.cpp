#include <iostream>

#include "DataBase.h"
#include "Parser.h"


using namespace std;

int main()
{
	try
	{
		Parser test;
		test.runOnSocket();
	}
	catch (char const* c)
	{
		cout << c << endl;
	}
	catch (...)
	{
		cout << "Unknown exception thrown" << endl;
	}	
}
