/*Ian Lofton Green
 *CS4280
 *Project 3
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>

using namespace std;

extern map<string, string> KWMap;
const int KWCount = 13;
const string KeyWords[KWCount] = {"begin", "end", "loop", "void", "var", "return", "in", "out", "program", "iffy", "then", "let", "data"};

extern map<string, string> opMap;
const int opCount = 18; //ignores == as it is handled elsewhere
const char operators[opCount] = {'=', '<', '>', ':', '+', '-', '*', '/', '%', '.', '(', ')', ',', '{', '}', ';', '[', ']'};

enum tkID {intTk, IDTk, opTk, keyTk, EOFTk};
const int tkCount = 5;

struct Token{
	tkID ID;
	int lineNumber;
	string description;
};

const char commentChar = '@';
extern int relativeIndex;
extern int tokenIndex;
extern int relativeLineIndex;

void buildKWMap();
int isKW(Token &);
void buildOpMap();
void getOp(Token &);
int isOp(char);
void display(Token);
#endif
