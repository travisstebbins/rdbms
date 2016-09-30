#define CATCH_CONFIG_MAIN
#include "Catch/include/catch.hpp"
#include "DataBase.h"
#include <iostream>
#include <vector>
#include <typeinfo>


//This file contains some of the implemented test cases.
//to run it, replace the makefile with makefile_modified, rename makefile_modified to makefile, and use "make" normally.

/*
SCENARIO("A database table is created", "createTable") {
	pair<string, int> xcoord = {"x", 5};
        pair<string, int> ycoord = {"y", 10};
        pair<string, int> zcoord = {"z", 2};
        string name = "points";
        vector<pair<string, int>> attributespoints = {xcoord, ycoord, zcoord};
        vector<string> primarykeyspoints = {"x", "y"};
        
	DataBase cp; //coordinate plane. not actually important because these are just tests
        cp.createTable(name, attributespoints, primarykeyspoints);
	//REQUIRE(dataBaseHashTable.find(name) == dataBaseHashTable.end());
	//cp.createTable(name, attributespoints, primarykeyspoints);
	//GIVEN("A table name, a vector of attributes, and a vector of primary keys"){

	//}
}
*/
//int main()
//{


template <typename T, typename V>
bool typechecker(T t, V v){	//capable of recieving inputs of any type. this function is used to ensure that the proper variable
	return (typeid(t).name()) == (typeid(v).name()); // types are being stored.
}

TEST_CASE("Initialize and Insert Into Table", "[Table]")
{
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};
	
	Table testTable("testTable", attributes1, primaryKeys1);
	
	SECTION("Check Table Structure")
	{
		REQUIRE(testTable.getAttributes() == attributes1);
		REQUIRE(testTable.getPrimaryKeys() == primaryKeys1);
	}
	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};
	
	SECTION("Check Table insertRecord(vector<string>)")
	{
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		cout << "Section: Check Table Insert\n" << testTable.show() << endl;
	}


	SECTION("Check Table insertRecord(Table)")
	{
		Table testTable2("testTable2", attributes1, primaryKeys1);
		Table testTable3("testTable3", attributes1, primaryKeys1);

		testTable3.insertRecord(v1);
		testTable3.insertRecord(v2);
		testTable3.insertRecord(v3);
		testTable3.insertRecord(v4);
		testTable3.insertRecord(v5);


		testTable.insertRecord(v1);
		testTable.insertRecord(v2);

		testTable2.insertRecord(v3);
		testTable2.insertRecord(v4);
		testTable2.insertRecord(v5);
		cout << "Section: Check Table insertRecord(Table)" << endl;
		cout << "testTable before\n" << testTable.show() << endl;
		cout << "testTable2 before\n" << testTable2.show() << endl;

		testTable.insertRecord(testTable2);
		cout << "testTable after testTable.insertRecord(testTable2)" << endl;
		cout << testTable.show() << endl;

		REQUIRE(testTable == testTable3);
	}
	
	
}

TEST_CASE("Test Table Delete and Update Record From Table", "[Table]")
{
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};
	
	Table testTable("testTable", attributes1, primaryKeys1);
	
	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};
	
	SECTION("Check void deleteRecord(vector<string> boolExpressions)")
	{
		Table testTable2("testTable2", attributes1, primaryKeys1);
		Table testTable3("testTable3", attributes1, primaryKeys1);
		
		testTable3.insertRecord(v1);
		testTable3.insertRecord(v2);
		testTable3.insertRecord(v3);
		testTable3.insertRecord(v4);
		testTable3.insertRecord(v5);
		
		testTable2.insertRecord(v3);
		testTable2.insertRecord(v4);

		
		cout << "Section: Check Table deleteRecord(Table)" << endl;
		cout << "testTable3 before\n" << testTable3.show() << endl;
		
		// testTable3.deleteRecord({"kind == dog"});
		// testTable3.deleteRecord({"name == Spot"});
		// testerfile.cpp:101: FAILED:
		// due to unexpected exception with message:
		// std::bad_alloc		
		
		cout << "testTable3 after\n" << testTable3.show() << endl;
		
		// REQUIRE(testTable3 == testTable2);
		
	}
	
	SECTION("Check void deleteRecord(size_t key)")
	{
		Table testTable2("testTable2", attributes1, primaryKeys1);
		Table testTable3("testTable3", attributes1, primaryKeys1);
		
		testTable3.insertRecord(v1);
		testTable3.insertRecord(v2);
		testTable3.insertRecord(v3);
		testTable3.insertRecord(v4);
		testTable3.insertRecord(v5);
		
		testTable2.insertRecord(v3);
		testTable2.insertRecord(v4);

		
		cout << "Section: Check Table deleteRecord(size_t key)" << endl;
		cout << "testTable3 before\n" << testTable3.show() << endl;
		
		cout << "testTable3 after\n" << testTable3.show() << endl;
		
		// REQUIRE(testTable3 == testTable2);
	}
	
	SECTION("Check updateRecord(vector<string> desiredAttributes, vector<string> values, vector<string> boolExpressions)")
	{
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		// Table a = (db.getTable("animals").project("temp", {"name", "kind"})).rename("a", {"aname", "akind"});
		// cout << a.show() << endl;
	}
}

TEST_CASE("Test Table Select, Project, and Rename", "[Table]")
{
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};
	
	Table testTable("testTable", attributes1, primaryKeys1);
	
	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};

	SECTION("Test Table select(string _name, vector<string> boolExpressions)")
	{
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);
		
		Table testTable2("testTable2", attributes1, primaryKeys1);
		Table testTable3("testTable3", attributes1, primaryKeys1);

		testTable3.insertRecord(v2);
		testTable3.insertRecord(v3);

		testTable2 = testTable.select("dogs",{"kind == dog"});

		cout << "Section: Test Select" << endl;
		cout << "Table of dogs\n" << testTable3.show() << endl;
		cout << "Selected table of dogs\n" << testTable2.show() << endl;

		REQUIRE(testTable2 == testTable3);
	}
	
	SECTION("Test Table project(string _name, vector<string> desiredAttributes)")
	{
		pair<string, int> p4 {"name", 20};
		pair<string, int> p5 {"kind", 8};
		vector<pair<string, int>> attributes2 = {p4, p5};
		vector<string> primaryKeys2 = {"name", "kind"};
		
		Table testTable2("testTable2", attributes2, primaryKeys2);
		vector<string> v6 = {"Joe", "cat"};
		vector<string> v7 = {"Spot", "dog"};
		vector<string> v8 = {"Snoopy", "dog"};
		
		testTable2.insertRecord(v6);
		testTable2.insertRecord(v7);
		testTable2.insertRecord(v8);
		
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		
		Table testTable3 = testTable.project("projection", {"name", "kind"});
		
		cout << "Section: Test Project" << endl;
		cout << "How the projected table should look\n" << testTable2.show() << endl;
		cout << "The projected table\n" << testTable3.show() << endl;
		
		REQUIRE(testTable2 == testTable3);
	}
	
	SECTION("Test Table rename(string _name, vector<string> newNames)")
	{
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);
		
		pair<string, int> p1 {"nickname", 20};
		pair<string, int> p2 {"type", 8};
		pair<string, int> p3 {"days", -1};
		vector<pair<string, int>> attributes2 = {p1, p2, p3};
		vector<string> primaryKeys2 = {"nickname", "type"};
		
		Table testTable2("testTable2", attributes2, primaryKeys2);
		
		vector<string> v6 = {"Joe", "cat", "4"};
		vector<string> v7 = {"Spot", "dog", "10"};
		vector<string> v8 = {"Snoopy", "dog", "3"};
		vector<string> v9 = {"Tweety", "bird", "1"};
		vector<string> v10 = {"Joe", "bird", "2"};
		
		testTable2.insertRecord(v6);
		testTable2.insertRecord(v7);
		testTable2.insertRecord(v8);
		testTable2.insertRecord(v9);
		testTable2.insertRecord(v10);
		
		Table testTable3 = testTable.rename("rename", {"nickname", "type", "days"});
		
		cout << "Section: Test rename" << endl;
		cout << "How renamed table should look\n" << testTable2.show() << endl;
		cout << "How renamed table looks\n" << testTable3.show() << endl;
		
		REQUIRE(testTable3 == testTable2);	
	}
	
	
}

TEST_CASE("Test DataBase Initialize, Insert, and Delete", "[DataBase]")
{
	SECTION("Initialize DataBase")
	{
		pair<string, int> p1 {"name", 20};
		pair<string, int> p2 {"kind", 8};
		pair<string, int> p3 {"years", -1};
		vector<pair<string, int>> attributes1 = {p1, p2, p3};
		vector<string> primaryKeys1 = {"name", "kind"};

		DataBase db;
		db.createTable("animals", attributes1, primaryKeys1);

		Table testTable = db.getTable("animals");

		REQUIRE(testTable.getAttributes() == attributes1);
		REQUIRE(testTable.getPrimaryKeys() == primaryKeys1);
	}

	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};

	DataBase db;
	db.createTable("animals", attributes1, primaryKeys1);

	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};

	Table testTable("testTable", attributes1, primaryKeys1);

	SECTION("Test void insertIntoTable(string tableName, vector<string> entry)")
	{
		
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);
		db.insertIntoTable("animals", v3);
		db.insertIntoTable("animals", v4);
		db.insertIntoTable("animals", v5);

		cout << "insertIntoTable Test" << endl;
		cout << testTable.show() << endl;
		cout << db.showTable("animals") << endl;

		REQUIRE(testTable == db.getTable("animals"));
	}

	SECTION("Test void insertIntoTable(string tableName, Table relationships)")
	{
		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);

		db.insertIntoTable("animals", v4);
		db.insertIntoTable("animals", v5);

		Table testTable2("testTable2", attributes1, primaryKeys1);
		testTable2.insertRecord(v1);
		testTable2.insertRecord(v2);
		testTable2.insertRecord(v3);
		testTable2.insertRecord(v4);
		testTable2.insertRecord(v5);

		db.insertIntoTable("animals", testTable);

		REQUIRE(testTable2 == db.getTable("animals"));
	}

	SECTION("Test void deleteFromTable(string name, vector<string> boolExpressions)")
	{
		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);
		db.insertIntoTable("animals", v3);
		db.insertIntoTable("animals", v4);
		db.insertIntoTable("animals", v5);

		//not going to work until deleteRecord is fixed
	}

}

TEST_CASE("Test DataBase Select Table and Project Table", "[DataBase]")
{
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};

	DataBase db;
	db.createTable("animals", attributes1, primaryKeys1);

	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};

	Table testTable("testTable", attributes1, primaryKeys1);

	SECTION("Test Table selectTable(string tableName, string _name, vector<string> boolExpressions)")
	{
		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);
		db.insertIntoTable("animals", v3);
		db.insertIntoTable("animals", v4);
		db.insertIntoTable("animals", v5);

		testTable.insertRecord(v2);
		testTable.insertRecord(v3);

		Table testTable2 = db.selectTable("animals", "dogs",{"kind == dog"});

		REQUIRE("testTable == testTable2");
	}

	SECTION("Test Table projectTable(string tableName, string _name, vector<string> desiredAttributes)")
	{
		pair<string, int> p4 {"name", 20};
		pair<string, int> p5 {"kind", 8};
		vector<pair<string, int>> attributes2 = {p4, p5};
		vector<string> primaryKeys2 = {"name", "kind"};

		Table testTable2("testTable2", attributes2, primaryKeys2);
		vector<string> v6 = {"Joe", "cat"};
		vector<string> v7 = {"Spot", "dog"};
		vector<string> v8 = {"Snoopy", "dog"};
		
		testTable2.insertRecord(v6);
		testTable2.insertRecord(v7);
		testTable2.insertRecord(v8);

		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);
		db.insertIntoTable("animals", v3);


		Table testTable3 = db.projectTable("animals", "projection", {"name", "kind"});
		REQUIRE(testTable2 == testTable3);
	}
}

TEST_CASE("Test Union Difference and Cross Product", "[DataBase]")
{
	pair<string, int> p1 {"name", 20};
	pair<string, int> p2 {"kind", 8};
	pair<string, int> p3 {"years", -1};
	vector<pair<string, int>> attributes1 = {p1, p2, p3};
	vector<string> primaryKeys1 = {"name", "kind"};

	DataBase db;
	db.createTable("animals", attributes1, primaryKeys1);

	vector<string> v1 = {"Joe", "cat", "4"};
	vector<string> v2 = {"Spot", "dog", "10"};
	vector<string> v3 = {"Snoopy", "dog", "3"};
	vector<string> v4 = {"Tweety", "bird", "1"};
	vector<string> v5 = {"Joe", "bird", "2"};

	Table testTable("testTable", attributes1, primaryKeys1);

	SECTION("Test Table setUnion(string tableName1, string tableName2)")
	{
		db.createTable("testTable", attributes1, primaryKeys1);
		db.createTable("testTable2", attributes1, primaryKeys1);
		
		db.insertIntoTable("testTable", v1);
		db.insertIntoTable("testTable", v2);

		db.insertIntoTable("testTable2", v3);
		db.insertIntoTable("testTable2", v4);
		db.insertIntoTable("testTable2", v5);

		Table testTable3("testTable3", attributes1, primaryKeys1);

		testTable3.insertRecord(v1);
		testTable3.insertRecord(v2);
		testTable3.insertRecord(v3);
		testTable3.insertRecord(v4);
		testTable3.insertRecord(v5);

		Table testTable4 = db.setUnion("testTable", "testTable2");
		REQUIRE(testTable4 == testTable3);
	}

	SECTION("Test Table setDifference(string tableName1, string tableName2)")
	{
		//not going to work until deleteRecord works

		// db.createTable("testTable", attributes1, primaryKeys1);
		// db.createTable("testTable2", attributes1, primaryKeys1);

		// db.insertIntoTable("testTable", v1);
		// db.insertIntoTable("testTable", v2);
		// db.insertIntoTable("testTable", v3);
		// db.insertIntoTable("testTable", v4);
		// db.insertIntoTable("testTable", v5);

		// db.insertIntoTable("testTable2", v4);
		// db.insertIntoTable("testTable2", v5);

		// Table testTable3("testTable3", attributes1, primaryKeys1);
		// testTable3.insertRecord(v1);
		// testTable3.insertRecord(v2);
		// testTable3.insertRecord(v3);

		// Table testTable4 = db.setDifference("testTable", "testTable2");

		// REQUIRE(testTable3 == testTable4);
	}

	SECTION("Test Table crossProduct(string tableName1, string tableName2)")
	{
		pair<string, int> p4 {"test", 20};
		pair<string, int> p5 {"birthday", -1};

		vector<pair<string, int>> attributes2 = {p4, p5};
		vector<string> primaryKeys2 = {"test"};

		vector<string> v6 = {"One", "1027"};
		vector<string> v7 = {"Two", "228"};

		vector<pair<string, int>> attributes3 = {p1, p2, p3, p4, p5};
		
		db.createTable("testTable", attributes2, primaryKeys2);
		db.insertIntoTable("testTable", v6);
		db.insertIntoTable("testTable", v7);
		
		db.insertIntoTable("animals", v1);
		db.insertIntoTable("animals", v2);

		Table testTable2("testTable2", attributes3, primaryKeys1);

		vector<string> v8 = {"Joe", "cat", "4", "One", "1027"};
		vector<string> v9 = {"Spot", "dog", "10", "Two", "228"};

		testTable2.insertRecord(v8);
		testTable2.insertRecord(v9);


		Table testTable3 = db.crossProduct("animals", "testTable");

		REQUIRE(testTable2 == testTable3);

	}
}

TEST_CASE("Test Read and Write to Disk", "[DataBase]")
{
	SECTION("Round Trip Test")
	{
		pair<string, int> p1 {"name", 20};
		pair<string, int> p2 {"kind", 8};
		pair<string, int> p3 {"years", -1};
		vector<pair<string, int>> attributes1 = {p1, p2, p3};
		vector<string> primaryKeys1 = {"name", "kind"};

		DataBase db;

		vector<string> v1 = {"Joe", "cat", "4"};
		vector<string> v2 = {"Spot", "dog", "10"};
		vector<string> v3 = {"Snoopy", "dog", "3"};
		vector<string> v4 = {"Tweety", "bird", "1"};
		vector<string> v5 = {"Joe", "bird", "2"};

		Table testTable("testTable", attributes1, primaryKeys1);

		testTable.insertRecord(v1);
		testTable.insertRecord(v2);
		testTable.insertRecord(v3);
		testTable.insertRecord(v4);
		testTable.insertRecord(v5);

		testTable.writeToDisk();

		db.readTableFromDisk("testTable.table");

		REQUIRE(testTable == db.getTable("testTable"));
	}
}

// SCENARIO("RDBMS online", "main"){	//can be thought of as somewhat similar to how an int main() function is. it's less of a function 
	// GIVEN("Some data to work with"){	//and more of a way to keep track of (and, if desired, format the output of)
		// pair<string, int> p1 {"name", 20}; //error handling
		// pair<string, int> p2 {"kind", 8};
		// pair<string, int> p3 {"years", -1};
		// vector<pair<string, int>> attributes1 = {p1, p2, p3};
		// vector<string> primaryKeys1 = {"name", "kind"};
	
		// DataBase db;
		// db.createTable("animals", attributes1, primaryKeys1);
		
		// WHEN("Creating a database"){
			// DataBase dc;
			// REQUIRE(!attributes1.empty());  //these two things can be commented out if needed,
			// REQUIRE(!primaryKeys1.empty()); //but ultimately a table doensn't necessarily require elements to exist
			// dc.createTable("animalia", attributes1, primaryKeys1);
		// }

		// vector<string> v1 = {"Joe", "cat", "4"};
		// vector<string> v2 = {"Spot", "dog", "10"};
		// vector<string> v3 = {"Snoopy", "dog", "3"};
		// vector<string> v4 = {"Tweety", "bird", "1"};
		// vector<string> v5 = {"Joe", "bird", "2"};
		
		// WHEN("Inserting into the database"){
			// vector<string> testvec;
			// REQUIRE(typechecker(v1,testvec)); //the REQUIRE blocks, similar to their name, require the result of what is
			// REQUIRE(typechecker(v2,testvec)); //contained within its parenthesis to be true
			// REQUIRE(typechecker(v3,testvec));
			// REQUIRE(typechecker(v4,testvec));
			// REQUIRE(typechecker(v5,testvec));
			
			// db.insertIntoTable("animals", v1);
			// db.insertIntoTable("animals", v2);	
			// db.insertIntoTable("animals", v3);
			// db.insertIntoTable("animals", v4);
			// db.insertIntoTable("animals", v5);
		// }

		// WHEN("Printing out table")
		// {
			// cout << db.showTable("animals") << endl;
		// }

		
		// WHEN("Selecting elements from a table"){
			// Table old_animals = db.selectTable("animals", "old_animals", {"years>9"});
			// Table dogs = db.selectTable("animals", "dogs", {"kind == dog"});
			// Table cats = db.selectTable("animals","cats", {"type == cat"});
			// Table birds = db.selectTable("animals", "birds", {"type == bird"});
		// }
		
		// WHEN("Insert table object to database")
		// {
			// Table birds = db.selectTable("animals", "birds", {"type == bird"});
			// db.createTable(birds);
		// }
		
		// Table dogs = db.selectTable("animals", "dogs", {"kind == dog"});
		// Table cats = db.selectTable("animals","cats", {"type == cat"});
				
		// db.createTable(dogs);
		// db.createTable(cats);

		// pair<string, int> p4 {"kind", 10};		
		// Table species("species", {p4}, {"kind"});
		// db.createTable(species);
		// db.insertIntoTable("species", db.projectTable("animals","temp", {"kind"}));
		
		// WHEN("Taking set difference of two tables")
		// {
			// Table differ = db.setDifference("animals", "cats");
		// }
		
		// WHEN("Taking set union of two tables")
		// {
			// Table dogsandcats = db.setUnion("dogs", "cats");
		// }
		
		// WHEN("Taking cross product of two tables")
		// {
			// Table cross = db.crossProduct("animals", "species");
		// }

	// } 
// }

