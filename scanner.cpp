/*Ian Lofton Green
 *CS4280
 *Project 3
*/

#include <string>
#include <cstring>
#include <iostream>
#include <cctype>
#include <stdlib.h>
#include <cstddef>
#include "scanner.h"
#include "token.h"

using namespace std;

int relativeLineIndex;
int relativeIndex;
bool commentState = false;
//string inputLine;
vector<string> fileString;

void scannerError(int error, string input) {
	if (error == errorNum) {
		cout << "Error: Tokens may not begin with a number. Number error occured on line (" << relativeLineIndex << ") at position (" << relativeIndex << ")." << endl;
	}
	else {
		cout << "Error: An unidentified error has occured.\nNo further information is available" << endl;
	}
}

string getString() {
	return fileString[relativeLineIndex];
}

int getCol(char input) {
	if (isalpha(input)) {
		return 0;
	}
	else if (isdigit(input)) {
		return 2;
	}
	else if (isOp(input)) {
		return 3;
	}
	else if (isspace(input)) {
		return 4;
	}
	else {
		cout << "Error: Failed to match input character (" << input << ") to FSA table column." << endl;
		return -1;
	}
}

bool isValidChar(char input) {
	if(!isalpha(input) && !isdigit(input) && !isOp(input)) {
		return false;
	}
	else {
		return true;
	}
}


int sanitizer(string &rawInput) {
	char space = ' ';
	char currentChar;
	char nextChar;
	char pastChar;
	string sanitizedInput;
	
	//cout << "\nRaw input: " << rawInput << endl;   //DIAGNOSTIC OUTPUT
	
	if (relativeIndex >= rawInput.length()) {
		return 0;
	}
	for (int n = relativeIndex; n < rawInput.length(); n++) {
		currentChar = rawInput.at(n);
		if (n > 0) {
			pastChar = rawInput.at(n-1);
		}
		if (currentChar == commentChar) {
			sanitizedInput.push_back(space);
			commentState = !commentState;
		}
		else if (!commentState) {
			if (isspace(currentChar)) {
				if (!isspace(pastChar)) {
					sanitizedInput.push_back(currentChar);
				}
			}
			else if (!isValidChar(currentChar)) {
				cout << "Error: Character (" << (relativeIndex + 1) << ") on line (" << relativeLineIndex << ") is invalid." << endl;
				return -1;
			}
			else {
				sanitizedInput.push_back(currentChar);
			}
		}
		relativeIndex++;
	}
	string wsDelim = "\n\r\t\f\v";
	size_t wsIndex = sanitizedInput.find_last_not_of(wsDelim);
	if (wsIndex != string::npos) {
		sanitizedInput.erase(wsIndex + 1);
	}
	else {
		bool lineComment = true;
		for (int i = 0; i < sanitizedInput.length(); i++) {
			if (sanitizedInput.at(i) == space) {
				lineComment = false;
				break;
			}
		}
		if (lineComment) {
			sanitizedInput.assign("");
		}
	}
//	inputLine.assign(sanitizedInput);
//	cout << "Valid string to be scanned: " << sanitizedInput << endl;   //DIAGNOSTIC OUTPUT
	return relativeIndex;
}


int tokenIndex;
int scanner(Token &token) {
	//token.lineNumber = relativeLineIndex;
	string newDescription;
	const char space = ' ';
	char nextChar;
	char lastChar;
	int nextInput;
	int currentState = 0;
	int nextState;
	string inputString = getString();

	if (tokenIndex == inputString.length()) {
		relativeLineIndex++;
		tokenIndex = 0;
		if (relativeLineIndex < fileString.size()) {
			inputString = fileString[relativeLineIndex];
		}
		else {
			token.description = "EOFTk";
			token.ID = EOFTk;
			token.lineNumber = (relativeLineIndex - 1);
			return 1;
		}
	}
	token.lineNumber = (relativeLineIndex + 1);
	while (tokenIndex <= inputString.length()) {
		if (tokenIndex < inputString.length()) {
			lastChar = nextChar;
			nextChar = inputString.at(tokenIndex);
		}
		else {
			nextChar = space;
		}
		nextInput = getCol(nextChar);
		nextState = FSATable[currentState][nextInput];
		if (nextState > 10) {
			token.description = newDescription;
			//cout << "\n" + nextState << endl;   /DIAGNOSTIC OUTPUT
			if (nextState == IDFinState) {
				if (isKW(token) != -1) {
					token.ID = keyTk;
				}
				else {
					token.ID = IDTk;
				}
			}
			else if (nextState == IntFinState) {
				token.ID = intTk;
			}
			else if (nextState == OpFinState) {
				if ((nextChar == '=') && (lastChar == '=')) {   //is ==
					token.ID = opTk;
					token.description.assign("eqeqTk"); //was assignTk
					getOp(token);
					tokenIndex++;   //skip next char
				}
				else {   //is just =
					token.ID = opTk;
					getOp(token);
				}
			}
			else {
				cout << "Error: Final state match could not be made." << endl;
				return -1;
			}
			return 0;
		}
		else if (nextState < 0) {
			currentState = 0;
			scannerError(nextState, inputString);
			tokenIndex++;
			exit(EXIT_FAILURE);
		}
		currentState = nextState;
		tokenIndex++;
		if (!isspace(nextChar)) {
			newDescription.push_back(nextChar);
		}
	}
	return -1;
}

void readFile(istream &stream) {
	Token token;
	buildKWMap();
	buildOpMap();
	string inputLine;
	relativeLineIndex = 0;
	while (getline(stream, inputLine)) {
		relativeIndex = 0;
		sanitizer(inputLine);
		if (inputLine.length() > 0) {
			fileString.push_back(inputLine);
		}
	}
}

