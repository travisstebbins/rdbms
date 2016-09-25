#include <iostream>

#include "DataBase.h"
#include "Catch/include/catch.hpp"

using namespace std;

int main()
{
	try
	{
		pair<string, int> p1 {"name", 20};
		pair<string, int> p2 {"kind", 8};
		pair<string, int> p3 {"years", -1};
		vector<pair<string, int>> attributes1 = {p1, p2, p3};
		vector<string> primaryKeys1 = {"name", "kind"};

		DataBase db;
		db.createTable("animals", attributes1, primaryKeys1);
		
		
		//Table animals("animals", attributes1, primaryKeys1);
		vector<string> v1 = {"Joe", "cat", "4"};
		vector<string> v2 = {"Spot", "dog", "10"};
		vector<string> v3 = {"Snoopy", "dog", "3"};
		vector<string> v4 = {"Tweety", "bird", "1"};
		vector<string> v5 = {"Joe", "bird", "2"};
		// animals.insertRecord(v1);
		// animals.insertRecord(v2);
		// animals.insertRecord(v3);
		// animals.insertRecord(v4);
		// animals.insertRecord(v5);
		
		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);
		db.insertIntoTable("animals", v3);
		db.insertIntoTable("animals", v4);
		db.insertIntoTable("animals", v5);
		cout << db.showTable("animals") << endl;
		//cout << animals.show() << endl;
		
		//Table animals = db.getTable("animals");
		//Table dogs = animals.select("dogs", {"kind == dog"});
		//cout << dogs.show() << endl;
		Table dogs = db.selectTable("animals", "dogs", {"kind == dog"});
		//Table dogs = db.getTable("animals").select("dogs", {"kind == dog"});
		db.createTable(dogs);
		cout << db.showTable("dogs") << endl;
		Table old_dogs = db.selectTable("dogs", "old_dogs", {"years>10"});
		cout << old_dogs.show() << endl;

		// Table cats = animals.select("cats", {"kind==cat"});
		Table cats = db.selectTable("animals","cats", {"kind==cat"});
		db.createTable(cats);
		cout << cats.show() << endl;

		Table projection = db.projectTable("animals", "projection", {"name", "kind"});
		cout << projection.show() << endl;

		pair<string, int> p4 {"kind", 10};		
		Table species("species", {p4}, {"kind"});
		db.createTable(species);
		db.insertIntoTable("species", db.projectTable("animals","temp", {"kind"}));
		cout << db.showTable("species") << endl;

		Table dogsandcats = db.setUnion("dogs", "cats");
		cout << dogsandcats.show() << endl;

		// this isn't working right
		// Table a = (animals.project("temp", {"name", "kind"})).rename("a", {"aname", "akind"});
		// cout << a.show() << endl;
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
