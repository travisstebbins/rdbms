#include <iostream>

#include "DataBase.h"
#include "Parser.h"


using namespace std;

int main(int argc, char **argv)
{
	try
	{
		Parser test;
		int c;
		while ((c = getopt (argc, argv, "cs")) != -1)
			switch(c)
			{
				case 'c':
					test.runOnCommandLine();
					break;
				case 's':
					test.runOnSocket();
					break;
				case '?':
					cout << "Usage: " << argv[0] << " -c (Run on command Line) or -s (Run as server)" << endl;
			}
		if (argv[optind] == NULL)
			cout << "Usage: " << argv[0] << " -c (Run on command Line) or -s (Run as server)" << endl;	
			
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
