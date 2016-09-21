#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class Table
{
	string name;
	unordered_map<string, tuple<> > tableHashMap;
	vector<pair<string, int> > attributes;
};