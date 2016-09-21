#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class Table
{
	string name;
	unordered_map<string, tuple<> > data;
	vector<pair<string, int> > attributes;
	vector<string> primaryKeys;
	
public:
	// constructors
	Table() {}
	Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys);
	// data manipulation functions
	Table select(vector<string> boolExpressions);
	Table project(vector<string> desiredAttributes);
	Table rename(vector<string> newNames);
	void show();
	void insertRecord(vector<string> entry);
	void insertRecord(Table relationship);
	void deleteRecord(vector<string> boolExpressions);
	void updateRecord(vecto<string> attributes, vector<string> values, vector<string> boolExpressions);
	void writeToDisk();
};