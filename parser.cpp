/*Ian Lofton Green
 *CS4280
 *Project 3 
*/

#include <iostream>
#include <stdlib.h>
#include "node.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"

using namespace std;

Token token;
Token emptyToken;
static string expectedToken;

void parserError() {
	cout << "Parser error on line( " << token.lineNumber << " ). Parser expectation: " << expectedToken << " Parser actual: " << token.description << endl;
	exit(EXIT_FAILURE);
}

node_t *createNode(string nonTerminal) {
	auto *node = new node_t();
	node->tag = nonTerminal;
	node->n1 = NULL;
	node->n2 = NULL;
	node->n3 = NULL;
	node->n4 = NULL;
	return node;
}

node_t *parser() {
	node_t *root = NULL;
	emptyToken.description = "Empty";
	scanner(token);
	root = program();
	if (token.ID != EOFTk) {
		expectedToken.assign("EOF");
		parserError();
	}
	else {
		cout << "Parser completed with no errors" << endl;
		return root;
	}
	return root;
}

//<program>  ->     <vars> <block>
node_t *program() {
	node_t *node = createNode("<program>");
	node->n1 = vars();
	node->n2 = block();
	return node;
}

//<block>       ->      begin <vars> <stats> end
node_t *block() {
	node_t *node = createNode("<block>");
	if ((token.ID == keyTk) && (token.description == "beginTk")) {
		node->tokens.push_back(token);
		scanner(token);
		node->n1 = vars();
		node->n2 = stats();
		if ((token.ID == keyTk) && (token.description == "endTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign("end");
			parserError();
		}
	}
	else {
		expectedToken.assign("begin");
		parserError();
	}
	return node;
}

//<vars>         ->      empty | data Identifier =  Integer  .  <vars>
node_t *vars() {
	node_t *node = createNode("<vars>");
	if ((token.ID == keyTk) && (token.description == "dataTk")) {
		scanner(token);
		if (token.ID == IDTk) {
			node->tokens.push_back(token);
			scanner(token);
			if ((token.ID == opTk) && (token.description == "assignTk")) {
				node->tokens.push_back(token);
				scanner(token);
				if (token.ID == intTk) {
					node->tokens.push_back(token);
					scanner(token);
					if ((token.ID == opTk) && (token.description == "periodTk")) {
						node->tokens.push_back(token);
						scanner(token);
						node->n1 = vars();
					}
					else {
						expectedToken.assign(".");
						parserError();
					}
				}
				else {
					expectedToken.assign("integer");
					parserError();
				}
			}
			else {
				expectedToken.assign("=");
				parserError();
			}
		}
		else {
			expectedToken.assign("identifier");
			parserError();
		}
	}
	else {
		node->tokens.push_back(emptyToken);
		return node;
	}
	return node;
}

//<expr>        ->      <N> - <expr>  | <N>
node_t *expr() {
	node_t *node = createNode("<expr>");
	node->n1 = N();
	if ((token.ID == opTk) && (token.description == "minusTk")) {
		node->tokens.push_back(token);
		scanner(token);
		node->n2 = expr();
	}
	return node;
}

//<N>             ->       <A> / <N> | <A> * <N> | <A>
node_t *N() {
	node_t *node = createNode("<N>");
	node->n1 = A();
	if (token.ID == opTk) {
		if (token.description == "slashTk") {
			node->tokens.push_back(token);
			scanner(token);
			node->n2 = N();
		}
		else if (token.description == "asteriskTk") {
			
			node->tokens.push_back(token);
			scanner(token);
			node->n2 = N();
		}		
	}
	return node;
}

//<A>             ->        <M> + <A> | <M>
node_t *A() {
	node_t *node = createNode("<A>");
	node->n1 = M();
	if ((token.ID == opTk) && (token.description == "plusTk")) {			
		node->tokens.push_back(token);
		scanner(token);
		node->n2 = A();
	}
	return node;
}

//<M>              ->     * <M> |  <R>
node_t *M() {
	node_t *node = createNode("<M>");
	if ((token.ID == opTk) && (token.description == "asteriskTk")) {
		node->tokens.push_back(token);
		scanner(token);
		node->n1 = M();
		return node;
	}
	else {
		node->n1 = R();
		return node;
	}
}

//<R>              ->      ( <expr> ) | Identifier | Integer
node_t *R() {
	node_t *node = createNode("<R>");
	if ((token.ID == opTk) && (token.description == "openParenTk")) {
		node->tokens.push_back(token);
		scanner(token);
		node->n1 = expr();
		if ((token.ID == opTk) && (token.description == "closeParenTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(")");
			parserError();
		}
	}
	else if ((token.ID == IDTk) || (token.ID == intTk)) {
		node->tokens.push_back(token);
		scanner(token);
		return node;
	}
	else {
		expectedToken.assign("( expr )  OR  identifier  OR  integer");
		parserError();
	}
	return node;
}

//<stats>         ->      <stat>  <mStat>
node_t *stats() {
	node_t *node = createNode("<stats>");
	node->n1 = stat();
	node->n2 = mstat();
	return node;

}

//<mStat>       ->      empty |  <stat>  <mStat>
node_t *mstat() {
	node_t *node = createNode("<mstat>");
	if ((token.ID == keyTk) && ((token.description == "inTk") || (token.description == "outTk") || (token.description == "beginTk") || (token.description == "iffyTk") || (token.description == "loopTk") || (token.description == "assignTk"))) {
		node->n1 = stat();
		node->n2 = mstat();
		return node;
	}
	else {
		node->tokens.push_back(emptyToken);
		return node;
	}
}

//<stat>           ->      <in> .  | <out> .  | <block> | <if> .  | <loop> .  | <assign> .  
node_t *stat() {
	node_t *node = createNode("<stat>");
	if ((token.ID == keyTk) && (token.description == "inTk")) {
		node->n1 = in();
		if ((token.ID == opTk) && (token.description == "periodTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(".");
			parserError();
		}
	}
	else if ((token.ID == keyTk) && (token.description == "outTk")) {
		node->n1 = out();
		if ((token.ID == opTk) && (token.description == "periodTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(".");
			parserError();
		}
	}
	else if ((token.ID == keyTk) && ((token.description == "beginTk"))) {
		node->n1 = block();
	}
	else if ((token.ID == keyTk) && (token.description == "iffyTk")) {
		node->n1 = iffy();
		if ((token.ID == opTk) && (token.description == "periodTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(".");
			parserError();
		}
	}
	else if ((token.ID == keyTk) && (token.description == "loopTk")) {
		node->n1 = loop();
		if ((token.ID == opTk) && (token.description == "periodTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(".");
			parserError();
		}
	}
	else if ((token.ID == IDTk)) {
		node->n1 = assign();
		if ((token.ID == opTk) && (token.description == "periodTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign(".");
			parserError();
		}
	}
	else {
		expectedToken.assign("<in> .  OR <out> .  OR <block> OR <iffy> .  OR <loop> .  OR <assign> .");
		parserError();
	}
	return node;
}

//<in>              ->      in  Identifier 
node_t *in() {
	node_t *node = createNode("<in>");
	if ((token.ID == keyTk) && (token.description == "inTk")) {
		node->tokens.push_back(token);
		scanner(token);
		if (token.ID == IDTk) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else {
			expectedToken.assign("Identifier");
			parserError();
		}
	}
	else {
		expectedToken.assign("in");
		parserError();
	}
	return node;
}

//<out>            ->      out <expr>
node_t *out() {
	node_t *node = createNode("<out>");
	if ((token.ID == keyTk) && (token.description == "outTk")) {
		node->tokens.push_back(token);
		scanner(token);
		node->n1 = expr();
		return node;
	}
	else {
		expectedToken.assign("out");
		parserError();
	}
	return node;
}

//<iffy>               ->      iffy [ <expr> <RO> <expr> ] then <stat>
node_t *iffy() {
	node_t *node = createNode("<iffy>");
	if ((token.ID == keyTk) && (token.description == "iffyTk")) {
		node->tokens.push_back(token);
		scanner(token);
		if ((token.ID == opTk) && (token.description == "openBracketTk")) {
			node->tokens.push_back(token);
			scanner(token);
			node->n1 = expr();
			node->n2 = RO();
			node->n3 = expr();
			if ((token.ID == opTk) && (token.description == "closeBracketTk")) {
				node->tokens.push_back(token);
				scanner(token);
				if ((token.ID == keyTk) && (token.description == "thenTk")) {
					node->tokens.push_back(token);
					scanner(token);
					node->n4 = stat();
					return node;
				}
				else {
					expectedToken.assign("then");
					parserError();
				}
			}
			else {
				expectedToken.assign("]");
				parserError();
			}
		}
		else {
			expectedToken.assign("[");
			parserError();
		}
	}
	else {			
		expectedToken.assign("iffy");
		parserError();
	}
	return node;
}

//<loop>          ->      loop  [ <expr> <RO> <expr> ]  <stat>
node_t *loop() {
	node_t *node = createNode("<loop>");
	if ((token.ID == keyTk) && (token.description == "loopTk")) {
		node->tokens.push_back(token);
		scanner(token);
		if ((token.ID == opTk) && (token.description == "openBracketTk")) {
			node->tokens.push_back(token);
			scanner(token);
			node->n1 = expr();
			node->n2 = RO();
			node->n3 = expr();
			if ((token.ID == opTk) && (token.description == "closeBracketTk")) {
				node->tokens.push_back(token);
				scanner(token);
				node->n4 = stat();
				return node;
			}
			else {
				expectedToken.assign("]");
				parserError();
			}
		}
		else {
			expectedToken.assign("[");
			parserError();
		}
	}
	else {			
		expectedToken.assign("loop");
		parserError();
	}
	return node;
}

//<assign>       ->      Identifier  = <expr> 
node_t *assign() {
	node_t *node = createNode("<assign>");
	if (token.ID == IDTk) {
		node->tokens.push_back(token);
		scanner(token);
		if ((token.ID == opTk) && (token.description == "assignTk")) {
			node->tokens.push_back(token);
			scanner(token);
			node->n1 = expr();
			return node;
		}
		else {
			expectedToken.assign("=");
			parserError();
		}
	}
	else {
		expectedToken.assign("Identifier");
		parserError();
	}
	return node;
}

//<RO>            ->      < | <  <  (two tokens >)  | >  | >  > (two tokens) |  == (one token ==) |   <  >    (two tokens)
node_t *RO() {
	node_t *node = createNode("<RO>");
	if ((token.ID == opTk) && (token.description == "lessThanTk")) {
		node->tokens.push_back(token);
		scanner(token);
		if ((token.ID == opTk) && (token.description == "lessThanTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else if ((token.ID == opTk) && (token.description == "greaterThanTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else if ((token.ID == opTk) && ((token.description != ("lessThanTk")) && (token.description != ("greaterThanTk")))) {
			expectedToken.assign("<< OR <>");
			parserError();
		}
		else {
			return node;
		}
	}
	else if ((token.ID == opTk) && (token.description == "greaterThanTk")) {
		node->tokens.push_back(token);
		scanner(token);
		if ((token.ID == opTk) && (token.description == "greaterThanTk")) {
			node->tokens.push_back(token);
			scanner(token);
			return node;
		}
		else if ((token.ID == opTk) && (token.description != "greaterThanTk")) {
			expectedToken.assign(">>");
			parserError();
		}
		else {
			return node;
		}
	}

	else if ((token.ID == opTk) && (token.description == "eqeqTk")) {
		node->tokens.push_back(token);
		scanner(token);
		return node;
	}
	else {
		expectedToken.assign("< OR << OR <> OR > OR >> OR ==");
		parserError();
	}
	return node;
}

