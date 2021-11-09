#include "hash.h"
#include "tokenizer.h"
#include <string.h>
int main(int argc, char *argv[]) 
{
  char* filename = argv[1];
  FILE* src_file = fopen(filename, "r");
  char* output = filename + strlen(filename)+3;
  strcpy(output, filename);
  output[strlen(output)-1] = '\0';
  strcat(output, "asm");
  printf("this is output %s\n", output);
  FILE* asmOutput = fopen(output, "w");
  char instr[MAX_TOKEN_LENGTH];
  char* tokenHold;
  token* theToken = NULL;
  if(src_file==NULL){
    printf(" failed: Null file! Null file!\n");
    return 2;
  }
  hash_struct* my_table = initTable(100);
  while(feof(src_file)!=1){
    fgets(instr, MAX_TOKEN_LENGTH, src_file);

    if (strcmp(instr, "\n") == 0 || instr[0]==';'){
      continue;
    }
    tokenHold = strtok(instr, "\n\t ");
    while(tokenHold !=NULL){
      if(tokenHold[0] == ';'){
        break;
      }
      theToken = init_token();
      read_token(theToken, tokenHold);
      write_token(theToken, asmOutput, my_table);
      print_token(theToken);
      delete_token(theToken);
      tokenHold = strtok(NULL, "\n\t ");
    }
    
  }
    done(my_table);
  fclose(asmOutput);
  fclose(src_file);
	/*token* nothing = init_token();
  nothing -> literal_value = 1;
  printf("%d el number \n", nothing->literal_value);
  nothing -> arg_no = 3;
  printf("%d el argo \n", nothing->arg_no);
  printf("my name is %u\n",nothing -> type);
  strcpy(nothing -> str, "yes");
  printf("Baarbara streisand %s\n", nothing -> str);
  
  delete_token(nothing);
  */
  return 0;
}
