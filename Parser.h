#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "DataBase.h"
#include "Table.h"
//we still need to include a LOT more stuff
//should we include DataBase.h here or in Parser.cpp?

//THINGS TO CONSIDER WHEN IMPLEMENTING: the inputs of each function, their types, and where in certain instructions those inputs can be found
//					more often than not, this information can be found in DataBase.cpp or wherever the function in question is stored

class Parser{
private:
	DataBase db;
public:
	//string<vector> commandHistory; //TODO: implement this and send it to a file maybe
	void commandOrQuery(string instruction);  //this is a relatively simple function. query instructions will always have an arrow "<-" somewhere in them (unless
						  //the query is part of (or rather, a source of input for) a command instruction) commandOrQuery will check either 
						  //for the existence of "<-", or the existence of "<" followed by "-", whichever is easier to implement.


	/* ---------- command functions ---------- */
	//TODO: implement a method (unless one has been implemented already) to keep track of all non-view tables currently open


	void commandParse(string instr); //gets the name of the table on which we are performing the command on
					 //parses through and looks for command instruction keywords: OPEN, CLOSE, WRITE, EXIT, SHOW, CREATE, UPDATE, INSERT, DELETE,
					 //DROP
					 //each of these keywords correspond to a command function. most of those functions take a table name, if any input at all.
					 //the create, update, insert, and delete functions will take a bit more work, as they all require more parsing to determine

	void commandOpen(string filename); //TODO: make a test case with the catch framework that REQUIRE()s the last six characters to be ".table"
	//				     TODO: do this for all commands involving file I/O
	//I'm assuming what this function does is read table info stored on a disk, then bring that table into scope by writing it into a table object that the program
	//can access
	//if this is not the case, I'll change the return type to Table so that I can write tables into scope as needed.

	void commandClose(string filename); //TODO: ask about a close function
					    //maybe just use drop table without saving first?
	
	void commandWrite(string filename); //TODO: ask Ryan how the Table::writeToDisk() function works

	void commandExit();		    //TODO: find a way to save all non-view tables before closing

	void commandShow(string tablename);
	
	void commandDrop(string tablename);

	//the following comment block concerns the insert function, and possibly the delete and update functions. but definitely the insert function.
	//
	//      the function will need to look for query syntax (if the command involves the result of the query, it will first need to find where
	//      in the instruction string that the query begins and ends. the  will then need to send that string containing that query to the
	//      queryParse, which will get a view table which will be used as an input for whichever command function called for the
	//      query (which is to say, whichever command function preceded the query call in the instruction string)
	//      the name of the query will be something like "cmd_query"  
	//

	void commandInsert(string instr); //this one is gonna be a bear to implement. see above string block for one example as to why.
	
	void commandDelete(string instr); //this one pretty much does the opposite of Insert, with the fortunate exception (hopefully) of
					  //not having to worry about getting information from a query

	void commandUpdate(string instr); //to implement this one, I'm thinking I just get the entry I'm trying to update, storing that in a
					  //string vector, modifying that string vector, calling Delete on the old entry, then using insert
					  //to put the new one in.


	/* --------------------------------------- */
	
	/* ---------- query functions ------------ */
	Table queryParse(string qname, string instr);	//first thing this does is check for nested queries
	
		
	/* --------------------------------------- */
};
