/*Ian Lofton Green
 *CS4280
 *Project 3
*/

#include <map>
#include <string>
#include <iostream>
#include "token.h"

map<string, string> KWMap;
void buildKWMap() {
	KWMap.insert(make_pair("begin", "beginTk"));
	KWMap.insert(make_pair("end", "endTk"));
	KWMap.insert(make_pair("loop", "loopTk"));
	KWMap.insert(make_pair("void", "voidTk"));
	KWMap.insert(make_pair("var", "varTk"));
	KWMap.insert(make_pair("return", "returnTk"));
	KWMap.insert(make_pair("in", "inTk"));
	KWMap.insert(make_pair("out", "outTk"));
	KWMap.insert(make_pair("program", "programTk"));
	KWMap.insert(make_pair("iffy", "iffyTk"));
	KWMap.insert(make_pair("then", "thenTk"));
	KWMap.insert(make_pair("let", "letTk"));
	KWMap.insert(make_pair("data", "dataTk"));
}
int isKW(Token &token) {
	for (int i = 0; i < KWCount; i++) {
		if (token.description == KeyWords[i]) {
			token.description = KWMap[token.description];
			return i;
		}
	}
	return -1;
}

map<string, string> opMap;
void buildOpMap() {
	opMap.insert(make_pair("=", "assignTk"));	
	opMap.insert(make_pair("==", "eqeqTk"));
	opMap.insert(make_pair("<", "lessThanTk"));
	opMap.insert(make_pair(">", "greaterThanTk"));
	opMap.insert(make_pair(":", "colonTk"));
	opMap.insert(make_pair("+", "plusTk"));
	opMap.insert(make_pair("-", "minusTk"));
	opMap.insert(make_pair("*", "asteriskTk"));
	opMap.insert(make_pair("/", "slashTk"));
	opMap.insert(make_pair("%", "modulusTk"));
	opMap.insert(make_pair(".", "periodTk"));
	opMap.insert(make_pair("(", "openParenTk"));
	opMap.insert(make_pair(")", "closeParenTk"));
	opMap.insert(make_pair(",", "commaTk"));
	opMap.insert(make_pair("{", "openBraceTk"));
	opMap.insert(make_pair("}", "closeBraceTk"));
	opMap.insert(make_pair(";", "semicolonTk"));
	opMap.insert(make_pair("[", "openBracketTk"));
	opMap.insert(make_pair("]", "closeBracketTk"));
}
void getOp (Token &token) {
	token.description.assign(opMap[token.description]);
}
int isOp(char character) {
	for (int i = 0; i < opCount; i++) {
		if (character == operators[i]) {
			return 1;
		}
	}
	return 0;
}

void display(Token token) {
	cout << "Line Number: < " << token.lineNumber << " > Token ID: < " +  token.description + " >";
	if (token.ID == EOFTk) {
		cout << " Description: < End Of File >" << endl;
	}
	else {
		cout << "\n";
	}
}

