/*Ian Lofton Green
 *CS4280
 *Project 3 
*/

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <vector>
#include "node.h"
#include "token.h"

using namespace std;

struct stack_t {
	Token token;
	int lineNumber;
};

extern vector<stack_t> stack;

void passFileName(string);
void printError(int);
void setTrigger();
int varScan(stack_t);
void varDefined(stack_t);
int idScan(stack_t);
void varID(stack_t);
void semScan(node_t*);
void displayStack();

#endif
