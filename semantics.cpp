/*Ian Lofton Green
 *CS4280
 *Project 3 
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include "semantics.h"

using namespace std;

vector<stack_t> stack;
vector<stack_t> nullStack;
static stack_t tempStack;
static int depthCount = 0;
static int depthTrigger = 0;
static int depthMax = 0;
string file;

void passFileName(string fileString) {
	file = fileString;
}

void setTrigger() {
	stack = nullStack;
	depthMax = depthCount;
	depthCount = 0;
	depthTrigger = 1;
}

void printError(int line) {
	string sysCall = "sed -n ";
	sysCall.append(to_string(line));
	sysCall.append("p ");
	sysCall.append(file);
	const char *command = sysCall.c_str();
	cout << "Line(" << line << "):" << endl;
	system(command);
}

int varScan(stack_t var) {
	for (int i = 0; i < stack.size(); i++) {
		if (stack.at(i).token.description == var.token.description) {
			return i;
		}
	}
	return -1;
}

void varDefined(stack_t var) {
	int varCount = varScan(var);
	if ((varCount >= 0) && (var.token.lineNumber != stack.at(varCount).token.lineNumber)){
		cout << "Error. Semantics: Variable (" <<  stack.at(varCount).token.description << ") on line " << var.token.lineNumber << " has already been defined on line " << stack.at(varCount).token.lineNumber << "." << endl;
		printError(stack.at(varCount).token.lineNumber);
		printError(var.token.lineNumber);
		exit(EXIT_FAILURE);
	}
	else {
		stack.push_back(var);
		displayStack();
	}
}
int idScan(stack_t var) {
	for (int i = 0; i < stack.size(); i++) {
		if (stack.at(i).token.description == var.token.description) {
			return 1;
		}
	}
	return 0;
}

void varID(stack_t var) {
	int idCount = idScan(var);
	if (idCount == 0) {
		cout << "Error. Semantics: New variable(" << var.token.description << ") on line " << var.token.lineNumber << " has not been defined." << endl;
		printError(var.token.lineNumber);
		exit(EXIT_FAILURE);
	}
}

void displayStack() {
	depthCount++;
	if ((depthTrigger == 1) && (depthCount == depthMax)) {
		cout << "Semantics verification completed with no errors." << endl;
		cout << "Variable stack: " << endl;
		for (int i = 0; i < stack.size(); i++) {
			cout << "\tVariable(" << i << ")  =  " << stack.at(i).token.description << endl;
		}
	}
}

void semScan(node_t *root) {
	if (root == NULL) {
		return;
	}
	if (root->tag == "<program>") {
		semScan(root->n1);
		semScan(root->n2);
		return;
	}
	if (root->tag == "<block>") {
		semScan(root->n1);
		semScan(root->n2);
	}
	if (root->tag == "<vars>") {
		tempStack.token = root->tokens.front();
		if (tempStack.token.description != "Empty") {
			varDefined(tempStack);
			semScan(root->n1);
		}
	}
	if (root->tag == "<expr>") {
		if (root->tokens.empty()) {
			semScan(root->n1);
		}
		else {
			semScan(root->n1);
			semScan(root->n2);
		}
	}
	if (root->tag == "<N>") {
		if (root->tokens.empty()) {
			semScan(root->n1);
		}
		else {
			semScan(root->n1);
			semScan(root->n2);
		}
	}
	if (root->tag == "<A>") {
		if (root->tokens.empty()) {
			semScan(root->n1);
		}
		else {
			semScan(root->n1);
			semScan(root->n2);
		}
	}
	if (root->tag == "<M>") {
		semScan(root->n1);
	}
	if (root->tag == "<R>") {
		tempStack.token = root->tokens.front();
		if (tempStack.token.ID == IDTk) {
			varID(tempStack);
			return;
		}
		else if (tempStack.token.ID == intTk) {	
			return;
		}
		else {
			semScan(root->n1);
		}
	}
	if (root->tag == "<stats>") {
		semScan(root->n1);
		semScan(root->n2);
	}
	if (root->tag == "<mstat>") {
		semScan(root->n1);
		semScan(root->n2);
	}
	if (root->tag == "<stat>") {
		semScan(root->n1);
	}
	if (root->tag == "<in>") {
		tempStack.token = root->tokens.front();
		if (tempStack.token.ID == IDTk) {
			varID(tempStack);
			return;
		}
	}
	if (root->tag == "<out>") {
		semScan(root->n1);
	}
	if (root->tag == "<iffy>") {
		semScan(root->n1);
		semScan(root->n2);
		semScan(root->n3);
		semScan(root->n4);
		
	}
	if (root->tag == "<loop>") {
		semScan(root->n1);
		semScan(root->n2);
		semScan(root->n3);
		semScan(root->n4);
	}
	if (root->tag == "<assign>") {
		tempStack.token = root->tokens.front();
		varID(tempStack);
		semScan(root->n1);
	}
}
