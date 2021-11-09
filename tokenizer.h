#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "token.h"
#include "hash.h"

//helper functions (implement these)


//theToken is the token to be populated
void read_token (token *theToken, char *instr);

//write to a file
void write_token(token *theToken, FILE* output, hash_struct* my_table);

//used for debugging
void print_token (token *theToken);


#endif
