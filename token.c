#include "token.h"
#include <stdlib.h>
#include <string.h>
token* init_token() 
{
  token* newToken = NULL;
  newToken = malloc(sizeof(token));
  newToken -> type = EMPTY;
  newToken -> literal_value = 0;
  newToken -> arg_no = 0;
  newToken -> str = malloc(sizeof(char)  *MAX_TOKEN_LENGTH);
  strcpy(newToken-> str, "");
  return newToken;
}

void delete_token(token* theToken)
{

	free(theToken-> str);
	free(theToken);
    //make sure you delete char* in token
}
