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

void commandOrQuery(string instruction);  //this is a relatively simple function. query instructions will always have an arrow "<-" somewhere in them (unless
					  //the query is part of (or rather, a source of input for) a command instruction) commandOrQuery will check either 
					  //for the existence of "<-", or the existence of "<" followed by "-", whichever is easier to implement.


/* --- command functions --- */
int commandParse(string instr);	//gets the name of the table on which we are performing the command on
				//parses through and looks for command instruction keywords: OPEN, CLOSE, WRITE, EXIT, SHOW, CREATE, UPDATE, INSERT, DELETE, DROP
				//each of these keywords correspond to a command function. most of those functions take a table name, if any input at all.
				//the create, update, insert, and delete functions will take a bit more work, as they all require more parsing to determine




//the following comment block concerns the insert function, and possibly the delete and update functions. but definitely the insert function.
//
//      the function will need to look for query syntax (if the command involves the result of the query, it will first need to find where
//      in the instruction string that the query begins and ends. the  will then need to send that string containing that query to the
//      queryParse, which will get a view table which will be used as an input for whichever command function called for the
//      query (which is to say, whichever command function preceded the query call in the instruction string)
//      the name of the query will be something like "cmd_query"  

/* ------------------------- */

/* --- query functions ----- */
Table queryParse(string qname, string instr);


/* ------------------------- */
