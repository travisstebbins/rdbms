#include <unordered_map>
#include "Table.h"
#include <string>
#include <vector>
#include <pair>

using namespace std;

class DataBase 
{
	unordered_map<String, Table> dataBaseHashTable;
	unordered_map<String, Table> viewHashTable;
	
public:

	void createTable(vector<pair<string, int>>, vector<string>);
	void dropTable(string tableName);
	Table setUnion(string tableName1, string tableName2);
	Table setDifference(string tableName1, string tableName2);
	Table cossProduct(string tableName1, string tableName2);
	Table getTable(string tableName);
	void readTableFromDisk(string fileName);
}