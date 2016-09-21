#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class Table
{
	unordered_map<string, tuple<>> tableHashMap;
	
public:
	//left empty so that database would compile
	Table() {}
	Table(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys) {}

};