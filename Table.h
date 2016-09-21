#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class Table
{
<<<<<<< HEAD
	string name;
	unordered_map<string, tuple<> > tableHashMap;
	vector<pair<string, int> > attributes;
=======
	unordered_map<string, tuple<>> tableHashMap;
	
public:
	//left empty so that database would compile
	Table() {}
	Table(string tableName, vector<pair<string, int>> attributes, vector<string> primaryKeys) {}

>>>>>>> bb5f7eb578404e3bab09f4e322a243fb009e8c1e
};