import socket
import sys
import socket
import re

SERVERADDRESS = 'localhost'
PORT = 1337

exit = 0#signifies when the program should exit
error_message = "We didn't quite understand that. Please check your inputs and try again."


def read_in():
	print" "
	print" "
	instruction = raw_input(">")
	inputsplit = instruction.split(' ') #splits the input string by whitespace
	print(chr(27) + "[2J")
	if(inputsplit[0].lower() == "add"):
		if (len(inputsplit) < 4):
			print error_message
			return
		#modify and send instruction socket to parser
		instr = 'INSERT INTO bank VALUES FROM (' + inputsplit[2] + ', ' + inputsplit[3] + ', 0, 0);' #creates sql command, values are set to 0
		sendToSocket(instr)
		
	elif(inputsplit[0].lower() == "delete"):#deletes user from bank table

		print" "
		print" "
		if (len(inputsplit) < 3):
			print error_message
			return
			
		instr = "DELETE FROM bank WHERE (accountNumber == " + inputsplit[2] + ");"#creates sql command
		sendToSocket(instr)
		
	elif(inputsplit[0].lower() == "update"):#Updates a user's specific balance for one of the accounts

		print" "
		print" "
		if (len(inputsplit) < 5):
			print error_message
			return
			
		instr1 = "currentbal <- project (" + inputsplit[3] + ", accountName) (select (accountNumber == " + inputsplit[2] + ") bank);" #Gets value of selected account
		instr2 = "DROP TABLE currentbal;"
		
		rtn = sendToSocket(instr1)
		sendToSocket(instr2)#drops view
		rtnSplit = rtn.split("data:")#cuts off fat of returned message
		rtnVal = int(re.sub(r"\D", "", rtnSplit[1]))#gets current value of bank account
		
		updBal = 0#the balance going into update parser command
		
		if ('(' in inputsplit[4]):
			updVal = 100 * int(re.sub(r"\D", "", inputsplit[4]))
			if (updVal > rtnVal):
				print "Account drained to $0.00"
			else:
				updBal = rtnVal - updVal#get new subtracted value
		else:
			updVal = 100 * int(inputsplit[4])
			updBal = rtnVal + updVal#get new added up value
		
		instr3 = 'UPDATE bank SET (' + inputsplit[3] + " = " + str(updBal) + ') WHERE (accountNumber == ' + inputsplit[2] + ');' 
		sendToSocket(instr3)

	elif(inputsplit[0].lower() == "display"):#Displays a specific user

		print" "
		print" "
		if (len(inputsplit) < 3):
			print error_message
			return
		
		instr1 = "thisUser <- select(accountNumber == " + inputsplit[2] + ") bank;"	#INSERT is the only command that can handle query inputs
		instr2 = "SHOW thisUser;"#splitting Display into these three instructions gets around this issue
		instr3 = "DROP TABLE thisUser;" #allows us to reuse thisUser without us having to restructure a significant amount of our code

		sendToSocket(instr1)
		rtnStr = sendToSocket(instr2)#prints our table
		sendToSocket(instr3)
		
		rtnStrSplit1 = rtnStr.split("\n")
		
		rtnStrSplit2 = rtnStrSplit1[1].split("\t")#gets first row needed

		rtnStrSplit3 = rtnStrSplit1[2].split("\t")#gets second row needed

		print "%-20s %-20s %-20s %-1s" % (rtnStrSplit2[0], rtnStrSplit2[2], rtnStrSplit2[4],rtnStrSplit2[6])
		print "-------------------------------------------------------------------------------"
		print "%-20s %-20s %-20s %-1s" % (rtnStrSplit3[0], rtnStrSplit3[2], rtnStrSplit3[4],rtnStrSplit3[6])
		print ""

	elif(inputsplit[0].lower() == "transfer"):#transfers money from selected account to other account
		
		#////////////////////////////BEGINS FIRST PART OF TRANSFER //////////////////////////////////
		print" "
		print" "
		if (len(inputsplit) < 5):
			print error_message
			return
	
		instr1 = "currentbal <- project (" + inputsplit[3] + ", accountName) (select (accountNumber == " + inputsplit[2] + ") bank);" #gets value of selected account
		instr2 = "DROP TABLE currentbal;"#drop view
		
		rtn = sendToSocket(instr1)
		sendToSocket(instr2)#drops view
		rtnSplit = rtn.split("data:")#cuts off fat of returned message
		rtnVal = int(re.sub(r"\D", "", rtnSplit[1]))#gets current value of bank account
		
		updBal = 0
		
		updVal = 100 * int(re.sub(r"\D", "", inputsplit[4]))
		if (updVal > rtnVal):
			return
		else:
			updBal = rtnVal - updVal
		
		instr3 = 'UPDATE bank SET (' + inputsplit[3] + " = " + str(updBal) + ') WHERE (accountNumber == ' + inputsplit[2] + ');' 
		sendToSocket(instr3)

		#////////////////////////////BEGINS Second PART OF TRANSFER /////////////////////////////////

		if(inputsplit[3] == "savings"):
			otherAccount = "checking"
		elif(inputsplit[3] == "checking"):
			otherAccount = "savings"
		else:
			print error_message
		
		instr4 = "currentbal <- project (" + otherAccount + ", accountName) (select (accountNumber == " + inputsplit[2] + ") bank);" #gets value of opposite account
       		rtn = sendToSocket(instr4)
		sendToSocket(instr2)#drops view
		rtnSplit = rtn.split("data:")#cuts off fat of returned message
		rtnVal = int(re.sub(r"\D", "", rtnSplit[1]))#gets current value of bank account
		
		updBal = rtnVal + updVal
		instr5 = 'UPDATE bank SET (' + otherAccount + " = " + str(updBal) + ') WHERE (accountNumber == ' + inputsplit[2] + ');' 
		sendToSocket(instr5)
		
	elif(inputsplit[0].lower() == "exit"):
		global exit
		print" "
		print" "
		instr = "EXIT;"
		exit = 1
	elif(inputsplit[0] == "Quit"):
		global exit
		print" "
		print" "
		instr = "EXIT;"
		exit = 1
		sendToSocket(instr)

	else:
		print error_message

def sendToSocket(msg):#sends to Parser.cpp
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serverAddress = (SERVERADDRESS, PORT);
	sock.connect(serverAddress)
	sock.send(msg)
	retMsg = sock.recv(1024)
	sock.close()
	return retMsg

def startup():
	print "      _                      _            ____                    _      " 
	print "     / \      __ _    __ _  (_)   ___    | __ )    __ _   _ __   | | __  "
	print "    / _ \    / _` |  / _` | | |  / _ \   |  _ \   / _` | | '_ \  | |/ /  " 
	print "   / ___ \  | (_| | | (_| | | | |  __/   | |_) | | (_| | | | | | |   <   "
	print "  /_/   \_\  \__, |  \__, | |_|  \___|   |____/   \__,_| |_| |_| |_|\_\  "
	print "             |___/   |___/                                               "

	print " "
	print "Welcome to Aggie Bank"
	print" "
	print" "
	instr = "CREATE TABLE bank (accountNumber INTEGER, accountName VARCHAR(20), savings INTEGER, checking INTEGER) PRIMARY KEY (accountName, accountNumber);"
	sendToSocket(instr)
	
def main():
	startup()#initiates startup screen
	
	
	while exit == 0:
		print "Enter a command from the following list of commands."
		print "Be sure to enter the right parameters!"
		print "--------------------------------------------------------------"
		print "Add User  <Account Number> <Account Name>"
		print "Delete User <Account Number>"
		print "Update Balance <Account Number> <Account(savings or checking)> <Money Amount> (put money subtractions in parenthesis like this: (12345))" 
		print "Display User <Account Number>"
		print "Transfer Money <Account Number> <Account(savings or checking)> <Money Amount>"
		print "Exit (Doesn't shut down rdbms)"
		print "Quit (Shuts down rdbms as well)"
		read_in()

main()
