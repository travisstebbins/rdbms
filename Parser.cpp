#include "Parser.h"
//should we include DataBase.h here or in Parser.h?

void Parser::commandOrQuery(string instruction)
{
	if(instruction.find("<-") == 0)// <- found 
	{
		if(instruction.find("INSERT") == 0)// <- found whatever command could use a query
		{
			commandParse(string instr);
		}
		else
		{
			//put query parse here
		}
	}
	else
	{
		commandParse(string instr);
	}
}	


void Parser::commandParse(string instr)//parses a command
{
	if(instruction.find("OPEN") == 0)// <- found 
	{
		instr = instr.erase(0,5);
		commandOpen(instr);// may change if the "OPEN " part of the string needs to be removed
	}
	else if(instruction.find("CLOSE") == 0)
	{
		instr = instr.erase(0,6);
		commandClose(instr);// may change if the "CLOSE " part of the string needs to be removed
	}
	else if(instruction.find("WRITE") == 0)
	{
		instr = instr.erase(0,6);
		commandWrite(instr);// may change if the "WRITE " part of the string needs to be removed
	}
	else if(instruction.find("SHOW") == 0)
	{
		instr = instr.erase(0,5);
		commandShow(instr);// may change if the "SHOW " part of the string needs to be removed
	}
	else if(instruction.find("DROP") == 0)
	{
		instr = instr.erase(0,5);
		commandDrop(instr);// may change if the "DROP " part of the string needs to be removed
	}
	else if(instruction.find("EXIT") == 0)
	{
		instr = instr.erase(0,5);
		commandExit();// may change if the "EXIT " part of the string needs to be removed
	}
	else
	{
		cout << "Not a valid command\n";
	}
}

void Parser::commandShow(string tablename)
{
	tablename = tablename.substr(0, tablename.size()-1);
	db.showTable(string tableName);
}


void Parser::commandWrite(string filename)
{
	
}

