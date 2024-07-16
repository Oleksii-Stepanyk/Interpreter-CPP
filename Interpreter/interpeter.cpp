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
		input.erase(remove(input.begin(), input.end(), ' '), input.end());

		regex digitRegex("\\d");
		regex tokensRegex("[\\(\\)]");
		regex operatorRegex("[\\+\\-\\*\\/]");
		regex functionRegex("(pow|abs|min|max)");

		int rightParenthesesCount = 0;
		int leftParenthesesCount = 0;

		for (int index = 0; index < input.size(); index++) {
			string currentChar(1, input[index]);
			string temp;
			if (regex_match(currentChar, digitRegex) ||
				(currentChar == "-" && (index == 0 ||
					regex_match(temp = string(1, input[index - 1]), operatorRegex) ||
					regex_match(temp = string(1, input[index - 1]), tokensRegex)))) {
				string number = currentChar;
				while (index + 1 < input.size() && (regex_match(temp = string(1, input[index + 1]), digitRegex))) {
					temp = string(1, input[++index]);
					number += temp;
				}

				tokens.push_back(number);
			}
			else if (regex_match(currentChar, tokensRegex) || regex_match(currentChar, operatorRegex)) {
				if (currentChar == "(") {
					if (input[index + 1] == ')') throw runtime_error("Parentheses are empty or wrong written");
					leftParenthesesCount++;
				}
				if (currentChar == ")") {
					rightParenthesesCount++;
				}
				tokens.push_back(currentChar);
			}
			else if (index + 2 < input.size() && regex_match(input.substr(index, 3), functionRegex)) {
				int leftParenthesesIndex = index + 3;
				if (input[leftParenthesesIndex] != '(') throw runtime_error("Values for cos/sin functions must be in parentheses");
				string function = currentChar + input[index + 1] + input[index + 2];
				index += 2;
				tokens.push_back(function);
			}
		}
		if (rightParenthesesCount != leftParenthesesCount) {
			throw runtime_error("Some parentheses are missing");
		}
		return tokens;
	}
};


class ShuntingYard {
private:
	int Precedence(string op) {
		if (op == "max" || op == "min") return 1;
		if (op == "+" || op == "-") return 2;
		if (op == "*" || op == "/") return 3;
		if (op == "pow") return 4;
		if (op == "abs") return 5;
		return 0;
	}

	bool IsLeftAssociative(string op) {
		if (op == "+" || op == "-" || op == "*" || op == "/") return true;
		return false;
	}

public:
	queue<string> toRPN(vector<string> tokens) {
		regex numberRegex("^-?[0-9]+$");
		regex operatorRegex("[\\+\\-\\*\\/]");
		regex functionRegex("(pow|abs|min|max)");

		queue<string> outputQueue = {};
		stack<string> operatorStack = {};

		for (int i = 0; i < tokens.size(); i++) {
			string token = tokens[i];
			if (regex_match(token, numberRegex)) {
				outputQueue.push(token);
			}
			else if (regex_match(token, functionRegex)) {
				operatorStack.push(token);
			}
			else if (regex_match(token, operatorRegex)) {
				while (!operatorStack.empty() && operatorStack.top() != "(" &&
					(Precedence(operatorStack.top()) > Precedence(token) ||
						(Precedence(operatorStack.top()) == Precedence(token) && IsLeftAssociative(token)))) {
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(token);
			}
			else if (token == "(") {
				operatorStack.push(token);
			}
			else if (token == ")") {
				while (operatorStack.top() != "(") {
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.pop();
				if (!operatorStack.empty() && regex_match(operatorStack.top(), functionRegex)) {
					outputQueue.push(operatorStack.top());
					operatorStack.pop();
				}
			}
		}
		while (!operatorStack.empty()) {
			outputQueue.push(operatorStack.top());
			operatorStack.pop();
		}
		return outputQueue;
	}
};


class Calculator {
private:
	int ApplyOperation(int firstOperand, int secondOperand, string op) {
		if (op == "+") return firstOperand + secondOperand;
		if (op == "-") return firstOperand - secondOperand;
		if (op == "*") return firstOperand * secondOperand;
		if (op == "/") {
			if (secondOperand == 0) {
				throw runtime_error("Cannot be divided by zero.");
			}
			return firstOperand / secondOperand;
		}
		if (op == "pow") return pow(firstOperand, secondOperand);
		if (op == "abs") return abs(firstOperand);
		if (op == "max") return max(firstOperand, secondOperand);
		if (op == "min") return min(firstOperand, secondOperand);
		return 0;
	}

public:
	int Calculate(queue<string> rpnQueue) {
		regex numberRegex("^-?[0-9]+$");
		regex operatorRegex("[\\+\\-\\*\\/]");
		stack<int> resultStack = {};

		while (!rpnQueue.empty()) {
			string token = rpnQueue.front();
			rpnQueue.pop();

			if (regex_match(token, numberRegex)) {
				int number = stoi(token);
				resultStack.push(number);
			}
			else if (token == "abs") {
				int firstOperand = resultStack.top();
				resultStack.pop();

				int result = ApplyOperation(firstOperand, 0, token);
				resultStack.push(result);
			}
			else if (token == "max" || token == "min" || token == "pow") {
				int secondOperand = resultStack.top();
				resultStack.pop();

				int firstOperand = resultStack.top();
				resultStack.pop();

				int result = ApplyOperation(firstOperand, secondOperand, token);
				resultStack.push(result);
			}
			else if (regex_match(token, operatorRegex) && resultStack.size() >= 2) {
				int secondOperand = resultStack.top();
				resultStack.pop();

				int firstOperand = resultStack.top();
				resultStack.pop();

				int result = ApplyOperation(firstOperand, secondOperand, token);
				resultStack.push(result);
			}
			else {
				cout << "Invalid token or insufficient operands for operation: " << token << endl;
				exit(1);
			}
		}
		if (!resultStack.empty()) {
			return resultStack.top();
		}
		else {
			cout << "No result found" << endl;
			exit(1);
		}
	}
};


void processInput(string input) {
	Tokenizer tokenizer;
	ShuntingYard shuntingYard;
	Calculator calculator;

	vector<string> tokens = tokenizer.Tokenize(input);
	queue<string> rpn = shuntingYard.toRPN(tokens);
	int result = calculator.Calculate(rpn);

	cout << result << endl;
}

int main() {
	string input;
	getline(cin, input);
	processInput(input);
	return 0;
}