#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> stringToTokens (string boolExpression) {
	// result
	vector<string> tokens;
	// remove new lines, tabs, and spaces
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\r'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\n'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), '\t'), boolExpression.end());
	boolExpression.erase(remove(boolExpression.begin(), boolExpression.end(), ' '), boolExpression.end());
	// convert string to tokens
	int current = 0;
	string token = "";
	while (current != boolExpression.length()) {
		 if (boolExpression[current] != '&' && boolExpression[current] != '|'
		 	&& boolExpression[current] != '(' && boolExpression[current] != ')')
		 {
		 	token += boolExpression[current++];
		 }
		 else
		 {
		 	if (token.length() > 0)
		 	{
		 		tokens.push_back(token);
		 	}
		 	token = "";
		 	if (boolExpression[current] == '(' || boolExpression[current] == ')')
		 	{
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 	else 
		 	{
		 		token += boolExpression[current++];
		 		token += boolExpression[current++];
		 		tokens.push_back(token);
		 		token = "";
		 	}
		 }
	}
	if (token.length() > 0)
	{
		tokens.push_back(token);
	}
	return tokens;
}

vector<string> convertBoolExpression (string boolExpression)
{
	vector<string> tokens = stringToTokens(boolExpression);
	vector<string> postfix;
	stack<string> opStack;
	for (int i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i][0] != '&' && tokens[i][0] != '|'
			&& tokens[i][0] != '(' && tokens[i][0] != ')')
		{
			postfix.push_back(tokens[i]);
		}
		else {
			if (tokens[i] == "(")
			{
				opStack.push(tokens[i]);
			}
			else if (tokens[i] == ")")
			{
				while (!opStack.empty() && opStack.top() != "(")
				{
					string op = opStack.top();
					opStack.pop();
					postfix.push_back(op);
				}
				if (!opStack.empty())
				{
					opStack.pop();
				}				
			}
			else if (!opStack.empty() && opStack.top() != "(")
			{
				string op = opStack.top();
				opStack.pop();
				opStack.push(tokens[i]);
				postfix.push_back(op);
			}
			else
			{
				opStack.push(tokens[i]);
			}
		}
	}
	// check if any operators are left on the opStack
	while (!opStack.empty())
	{
		postfix.push_back(opStack.top());
		opStack.pop();
	}
	return postfix;
}

string print (vector<string> vec)
{
	string result = "{ ";
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i != vec.size() - 1)
		{
			result += vec[i] + " , "; 
		}
		else
		{
			result += vec[i] + " }";
		}
	}
	return result;
}

int main ()
{
	string boolExpression1 = "name == Bob && age > 10 || (kind == dog && height <= 30)";
	vector<string> tokens1 = stringToTokens(boolExpression1);
	vector<string> postfix1 = convertBoolExpression(boolExpression1);
	cout << "Tokens:" << endl;
	cout << print(tokens1) << endl;
	cout << "Postfix:" << endl;
	cout << print(postfix1) << endl;

	string boolExpression2 = "(v1==hello || (v2!= 5 && v3<=5)) && v4 > 7 && (v5<=6 || v6 >=-5) || v7 != hello world";
	vector<string> tokens2 = stringToTokens(boolExpression2);
	vector<string> postfix2 = convertBoolExpression(boolExpression2);
	cout << "Tokens:" << endl;
	cout << print(tokens2) << endl;
	cout << "Postfix:" << endl;
	cout << print(postfix2) << endl;

	string boolExpression3 = "aname == name && akind != kind";
	vector<string> tokens3 = stringToTokens(boolExpression3);
	vector<string> postfix3 = convertBoolExpression(boolExpression3);
	cout << "Tokens:" << endl;
	cout << print(tokens3) << endl;
	cout << "Postfix:" << endl;
	cout << print(postfix3) << endl;

	string boolExpression4 = "kind == \"dog\"";
	vector<string> tokens4 = stringToTokens(boolExpression4);
	vector<string> postfix4 = convertBoolExpression(boolExpression4);
	cout << "Tokens:" << endl;
	cout << print(tokens4) << endl;
	cout << "Postfix:" << endl;
	cout << print(postfix4) << endl;
}

// result1
// { name==Bob , age>10 , && , kind==dog , height<=30 , && , || }

// result2
// { v1==hello , v2!=5 , v3<=5 , && , || , v4>7 , && , v5<=6 , v6>=-5 , || , && , v7!=helloworld , || }