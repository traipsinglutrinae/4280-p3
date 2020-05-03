/*Ian Lofton Green
 *CS4280
 *Project 3 
*/

#ifndef PARSER_H
#define PARSER_H

using namespace std;

extern Token token;
extern Token emptyToken;
node_t *parser();
node_t *createNode(string);
void parseError();

node_t *program();
node_t *block();
node_t *vars();
node_t *expr();
node_t *N();
node_t *A();
node_t *M();
node_t *R();
node_t *stats();
node_t *mstat();
node_t *stat();
node_t *in();
node_t *out();
node_t *iffy();
node_t *loop();
node_t *assign();
node_t *RO();

#endif
