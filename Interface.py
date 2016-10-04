import socket
import sys
#TODO: figure out how to "include" the c++ parser, if we even need to do that at all

print "      _                      _            ____                    _      " 
print "     / \      __ _    __ _  (_)   ___    | __ )    __ _   _ __   | | __  "
print "    / _ \    / _` |  / _` | | |  / _ \   |  _ \   / _` | | '_ \  | |/ /  " 
print "   / ___ \  | (_| | | (_| | | | |  __/   | |_) | | (_| | | | | | |   <   "
print "  /_/   \_\  \__, |  \__, | |_|  \___|   |____/   \__,_| |_| |_| |_|\_\  "
print "             |___/   |___/                                               "

print " "
print "Welcome to Aggie Bank"
print "Enter a command from the following list of commands."
print "Be sure to enter the right parameters!"
print "--------------------------------------------------------------"
print "Add User  <Account Number> <Account Name>"
print "Delete User <Account Number>"
print "Update Balance <Account Number> <Account(Savings or Checking)> <Money Amount>"
print "Display User <Account Number>"
print "Tansfer Money <Account Number> <Account(Savings or Checking)> <Money Amount>"

error_message = "We didn't quite understand that. Please check your inputs and try again."

#TODO: add socket functionality
#TODO: comment out all pass statements as the functions are implemented

#TODO: make read_in() return something aftet each inputted line
def read_in(): #WARNING: when implementing or modifying the recursion in this function, be sure that it doesn't cause too much recursion, which is a thing that causes run time errors.
	for line in sys.stdin:
		instruction = line #not necessary, but I think it looks nicer
		inputsplit = line.split(' ') #splits the input string by whitespace
		if(inputsplit[0] == "Add"):
			#modify and send instruction socket to parser
			pass #tells the compiler to "pass by" without doing anything
		elif(inputsplit[0] == "Delete"):
			#modify and send instruction socket to parser
			pass
		elif(inputsplit[0] == "Update"):
			#modify and send instruction socket to parser
			pass
		elif(inputsplit[0] == "Display"):
			#modify and send instruction socket to parser
			pass
		elif(inputsplit[0] == "Transfer"):
			#modify and send instruction socket to parser
			pass
		else:
			print error_message

read_in()
