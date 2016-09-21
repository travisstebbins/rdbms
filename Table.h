#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class Table
{
	string name;
	unordered_map<size_t, tuple<> > data;
	vector<pair<string, int> > attributes;
	vector<string> primaryKeys;
	vector<int> primaryKeyIndices;

	// private functions
	bool evaluate(tuple<> entry, vector<string> boolExpressions);
	void insertRecord(tuple<> entry);
	
public:
	// constructors
	Table() {}
	Table(string _name, vector<pair<string, int>> _attributes, vector<string> _primaryKeys);
	// data manipulation functions
	Table select(string _name, vector<string> boolExpressions);
	Table project(string _name, vector<string> desiredAttributes);
	Table rename(string _name, vector<string> newNames);
	string show();
	void insertRecord(vector<string> entry);
	void insertRecord(Table relationship);
	void deleteRecord(vector<string> boolExpressions);
	void updateRecord(vector<string> attributes, vector<string> values, vector<string> boolExpressions);
	void writeToDisk();
};