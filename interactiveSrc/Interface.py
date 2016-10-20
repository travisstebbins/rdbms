import socket
import sys
import socket
import re

SERVERADDRESS = 'localhost'
PORT = 10018

exit = 0 #signifies when the program should exit
error_message = "Please enter a valid option."

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
	instr = "CREATE TABLE bank (accountNumber INTEGER, firstName VARCHAR(20), lastName VARCHAR(20), savings INTEGER, checking INTEGER) PRIMARY KEY (accountNumber);"
	sendToSocket(instr)

def addUser():
	print "Add User"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	firstName = raw_input("First Name: ")
	lastName = raw_input("Last Name: ")
	instr = "INSERT INTO bank VALUES FROM (" + accountNumber + ", \"" + firstName + "\", \"" + lastName + "\", 0, 0);"
	result = sendToSocket(instr)
	if (result == "Failure"):
		print "Error adding user"
		return

def deleteUser():
	print "Delete User"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	instr = "DELETE FROM bank WHERE (accountNumber == " + accountNumber + ");"
	result = sendToSocket(instr)
	if (result == "Failure"):
		print "Error deleting user"
		return

def depositMoney():
	print "Deposit Money"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	accountType = ""
	while (accountType != "savings" and accountType != "checking"):
		print "1) Savings"
		print "2) Checking"
		accountType = raw_input("Option: ")
		if (accountType == "1"):
			accountType = "savings"
		elif (accountType == "2"):
			accountType = "checking"
		else:
			print error_message
	depositAmount = eval(raw_input("Deposit Amount: "))

	instr1 = "currentbal <- project (" + accountType + ", accountNumber) (select (accountNumber == " + accountNumber + ") bank);" #Gets value of selected account
	rtn = sendToSocket(instr1)
	if (rtn == "Failure"):
		print "Error depositing money"
		return;
	instr2 = "DROP TABLE currentbal;"
	sendToSocket(instr2)#drops view

	rtnSplit = rtn.split("data:")#cuts off fat of returned message
	if (len(rtnSplit) == 1):
		print "Account does not exist"
		return
	rtnSplit2 = rtnSplit[1].split(",")
	if (len(rtnSplit2) == 1):
		print "Account does not exist"
		return
	rtnVal = int(rtnSplit2[1]) #gets current value of bank account
	updBal = 0 #the balance going into update parser command
	updVal = 100 * depositAmount
	updBal = rtnVal + updVal
	instr3 = "UPDATE bank SET (" + accountType + " = " + str(updBal) + ") WHERE (accountNumber == " + accountNumber + ");"
	result = sendToSocket(instr3)
	if (result == "Failure"):
		print "Error depositing money"
		return

def withdrawMoney():
	print "Withdraw Money"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	accountType = ""
	while (accountType != "savings" and accountType != "checking"):
		print "1) Savings"
		print "2) Checking"
		accountType = raw_input("Option: ")
		if (accountType == "1"):
			accountType = "savings"
		elif (accountType == "2"):
			accountType = "checking"
		else:
			print error_message
	withdrawAmount = eval(raw_input("Withdraw Amount: "))

	instr1 = "currentbal <- project (" + accountType + ", accountNumber) (select (accountNumber == " + accountNumber + ") bank);" #Gets value of selected account
	rtn = sendToSocket(instr1)
	if (rtn == "Failure"):
		print "Error withdrawing money"
		return;
	instr2 = "DROP TABLE currentbal;"
	sendToSocket(instr2)#drops view

	rtnSplit = rtn.split("data:")#cuts off fat of returned message
	if (len(rtnSplit) == 1):
		print "Account does not exist"
		return
	rtnSplit2 = rtnSplit[1].split(",");
	if (len(rtnSplit2) == 1):
		print "Account does not exist"
		return
	rtnVal = int(rtnSplit2[1]) #gets current value of bank account
	updBal = 0 #the balance going into update parser command
	updVal = 100 * withdrawAmount
	if (updVal >= rtnVal):
		print "Account drained to $0.00"
	else:
		updBal = rtnVal - updVal#get new added up value
	instr3 = "UPDATE bank SET (" + accountType + " = " + str(updBal) + ") WHERE (accountNumber == " + accountNumber + ");"
	result = sendToSocket(instr3)
	if (result == "Failure"):
		print "Error withdrawing money"
		return

def displayUser():
	print "Display User"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	instr1 = "thisUser <- select (accountNumber == " + accountNumber + ") bank;" #INSERT is the only command that can handle query inputs
	instr2 = "SHOW thisUser;" #splitting Display into these three instructions gets around this issue
	instr3 = "DROP TABLE thisUser;" #allows us to reuse thisUser without us having to restructure a significant amount of our code

	result = sendToSocket(instr1)
	if (result == "Failure"):
		print "Error displaying user"
		return
	rtnStr = sendToSocket(instr2) #prints the table
	if (rtnStr == "Failure"):
		print "Error displaying user"
		return
	sendToSocket(instr3)

	rtnStrSplit1 = rtnStr.split("\n")
	rtnStrSplit2 = rtnStrSplit1[1].split("\t\t") #gets first row needed
	rtnStrSplit3 = rtnStrSplit1[3].split("\t\t") #gets second row needed

	if(len(rtnStrSplit3) == 1):
		print "Account does not exist"
		return
	
	print "%-15s %-20s %-20s %20s %20s" % ("Account #", "First Name", "Last Name", "Savings", "Checking")
	print "-----------------------------------------------------------------------------------------------------------"
	print "%-15s %-20s %-20s %20.2f %20.2f" % (rtnStrSplit3[0], rtnStrSplit3[1], rtnStrSplit3[2], int(rtnStrSplit3[3]) / 100.0, int(rtnStrSplit3[4]) / 100.0)
	print ""

def transferMoney():
	print "Transfer Money"
	print "------------------------"
	accountNumber = raw_input("Account Number: ")
	accountType = ""
	otherAccount = ""
	while (accountType != "savings" and accountType != "checking"):
		print "1) From Savings to Checking"
		print "2) From Checking to Savings"
		accountType = raw_input("Option: ")
		if (accountType == "1"):
			accountType = "savings"
			otherAccount= "checking"
		elif (accountType == "2"):
			accountType = "checking"
			otherAccount = "savings"
		else:
			print error_message
	transferAmount = eval(raw_input("Transfer Amount: "))

	#////////////////////////////BEGINS FIRST PART OF TRANSFER //////////////////////////////////
	instr1 = "currentbal <- project (" + accountType + ", accountNumber) (select (accountNumber == " + accountNumber + ") bank);" #gets value of selected account
	instr2 = "DROP TABLE currentbal;" #drop view

	rtn = sendToSocket(instr1)
	if (rtn == "Failure"):
		print "Error transferring money"
		return
	sendToSocket(instr2) #drops view

	rtnSplit = rtn.split("data:")#cuts off fat of returned message
	if (len(rtnSplit) == 1):
		print "Account does not exist"
		return
	rtnSplit2 = rtnSplit[1].split(",")
	if (len(rtnSplit2) == 1):
		print "Account does not exist"
		return
	rtnVal = int(rtnSplit2[1]) #gets current value of bank account

	updBal = 0

	updVal = 100 * transferAmount

	if (updVal > rtnVal):
		print "Account overdrawn"
		return
	else:
		updBal = rtnVal - updVal
	instr3 = "UPDATE bank SET (" + accountType + " = " + str(updBal) + ") WHERE (accountNumber == " + accountNumber + ");" 
	result = sendToSocket(instr3)
	if (result == "Failure"):
		print "Error transferring money"
		return

	#////////////////////////////BEGINS Second PART OF TRANSFER /////////////////////////////////
	instr4 = "currentbal <- project (" + otherAccount + ", accountNumber) (select (accountNumber == " + accountNumber + ") bank);" #gets value of opposite account

   	rtn = sendToSocket(instr4)
   	if (rtn == "Failure"):
   		print "Error transferring money"
   		return
	sendToSocket(instr2) #drops view

	rtnSplit = rtn.split("data:")#cuts off fat of returned message
	if (len(rtnSplit) == 1):
		print "Account does not exist"
		return
	rtnSplit2 = rtnSplit[1].split(",")
	if (len(rtnSplit2) == 1):
		print "Account does not exist"
		return
	rtnVal = int(rtnSplit2[1]) #gets current value of bank account
	
	updBal = rtnVal + updVal
	instr5 = "UPDATE bank SET (" + otherAccount + " = " + str(updBal) + ") WHERE (accountNumber == " + accountNumber + ");" 
	result = sendToSocket(instr5)
	if (result == "Failure"):
		print "Error transferring money"
		return

def displayAllUsers():
	print "Display All Users"
	print "------------------------"
	instr1 = "allUsers <- select (accountNumber > 0) bank;"
	instr2 = "SHOW allUsers;" 
	instr3 = "DROP TABLE allUsers;"

	result = sendToSocket(instr1)
	if (result == "Failure"):
		print "Error displaying all users"
		return
	rtnStr = sendToSocket(instr2)
	if (rtnStr == "Failure"):
		print "Error displaying all users"
		return
	sendToSocket(instr3)

	rtnStrSplit1 = rtnStr.split("\n")
	rtnStrSplit2 = rtnStrSplit1[1].split("\t\t") #gets attribute names

	print "%-15s %-20s %-20s %20s %20s" % ("Account #", "First Name", "Last Name", "Savings", "Checking")
	print "-----------------------------------------------------------------------------------------------------------"
	for i in range(3,len(rtnStrSplit1) - 1):
		rtnStrSplit3 = rtnStrSplit1[i].split("\t\t") #gets row of data
		if(len(rtnStrSplit3) == 1):
			print "Account does not exist"
			return	
		print "%-15s %-20s %-20s %20.2f %20.2f" % (rtnStrSplit3[0], rtnStrSplit3[1], rtnStrSplit3[2], int(rtnStrSplit3[3]) / 100.0, int(rtnStrSplit3[4]) / 100.0)
	print ""

def disconnect():
	global exit
	exit = 1
	print "Disconnecting"

def shutDown():
	instr = "EXIT;"
	sendToSocket(instr)
	print "Shutting down database"
	
def main():
	startup() #initiates startup screen
	
	while exit == 0:
		print "What would you like to do? (Enter the option number)"
		print "--------------------------------------------------------------"
		print "1) Add a new user"
		print "2) Delete a user"
		print "3) Deposit money"
		print "4) Withdraw money" 
		print "5) Display a user"
		print "6) Transfer money"
		print "7) Display all users"
		print "8) Disconnect"
		print "9) Disconnect and shut down database"

		option = raw_input("Option: ")
		print ""
		if (option == "1"):
			addUser()
		elif (option == "2"):
			deleteUser()
		elif (option == "3"):
			depositMoney()
		elif (option == "4"):
			withdrawMoney()
		elif (option == "5"):
			displayUser()
		elif (option == "6"):
			transferMoney()
		elif (option == "7"):
			displayAllUsers()
		elif (option == "8"):
			disconnect()
		elif (option == "9"):
			shutDown()
			disconnect()
		else:
			print error_message
		print ""

main()