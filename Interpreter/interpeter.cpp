#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <queue>
using namespace std;

class Tokenizer {
public:
	vector<string> Tokenize(string input) {
		vector<string> tokens = {};
		remove(input.begin(), input.end(), ' ');

		regex digitRegex("\\d");
		regex tokensRegex("[\\(\\)]");
		regex operatorRegex("[\\+\\-\\*\\/]");
		regex functionRegex("(pow)|(abs)|(min)|(max)");

		int rightParenthesesCount = 0;
		int leftParenthesesCount = 0;

		for (int index = 0; index < input.size(); index++)
		{
			string currentChar(1, input[index]);
			string temp;
			if (regex_match(currentChar, digitRegex) ||
				(currentChar == "-" && (index == 0 || regex_match(temp = (1, input[index - 1]), operatorRegex))))
			{
				string number = currentChar;
				while (index + 1 < input.size() && (regex_match(temp = (1, input[index + 1]), digitRegex)))
				{
					temp = (1, input[++index]);
					number += temp;
				}

				tokens.push_back(number);
			}
			else if (regex_match(currentChar, tokensRegex) || regex_match(currentChar, operatorRegex))
			{
				if (currentChar == "(") {
					if (input[index + 1] == ')') throw runtime_error("Parentheses are empty or wrong written");
					leftParenthesesCount++;
				}
				if (currentChar == ")") {
					rightParenthesesCount++;
				}
				tokens.push_back(currentChar);
			}
			else if (index + 2 < input.size() && regex_match((currentChar + input[index + 1] + input[index + 2]), functionRegex))
			{
				int leftParenthesesIndex = index + 3;
				if (input[leftParenthesesIndex] != '(') throw runtime_error("Values for cos/sin functions must be in parentheses");
				string function = currentChar + input[index + 1] + input[index + 2];
				index += 2;
				tokens.push_back(function);
			}
		}
		if (rightParenthesesCount != leftParenthesesCount)
		{
			throw runtime_error("Some parentheses are missing");
		}
		return tokens;
	}
};

int main()
{
	Tokenizer tokenizer;
	string input;
	getline(cin, input);
	vector<string> tokens = tokenizer.Tokenize(input);
	for (string token : tokens)
	{
		cout << token << " ";
	}
	cout << endl;
	return 0;
}