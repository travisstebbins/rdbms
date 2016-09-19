#include <unordered_map>
#include "Table.h"
#include <string>

using namespace std;

class DataBase 
{
	unordered_map<String, Table> dataBaseHashTable;
	unordered_map<String, Table> viewHashTable;
	
public:

	void createTable()
}