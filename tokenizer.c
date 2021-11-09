#include "tokenizer.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "hash.h"
#include <stdbool.h>
#include <assert.h>

int defun = 0;
char routine[2000] = {0};
int BR = 0;
int ifNum = 0;
int finalIf= 0;
int curr = 0;
int branches = 0;
//theToken is the token to be populated
void read_token (token *theToken,  char *instr){
  int length = strlen(instr);
  if(!strcmp(instr, "defun")){
    theToken -> type = DEFUN;
    strcpy(theToken->str, instr);
  } else if(!strcmp(instr, "return")) {
    theToken -> type = RETURN;
    strcpy(theToken->str, instr);
  } else if(!strcmp(instr, "+")) {
    theToken -> type = PLUS;
    strcpy(theToken->str, instr);
  } else if(!strcmp(instr, "-")) {
    theToken -> type = MINUS;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "*")) {
    theToken -> type = MUL;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "/")) {
    theToken -> type = DIV;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "%")) {
    theToken -> type = MOD;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "and")) {
    theToken -> type = AND;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "or")) {
    theToken -> type = OR;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "not")) {
    theToken -> type = NOT;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "xor")) {
    theToken -> type = XOR;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "lt")) {
    theToken -> type = LT;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "le")) {
    theToken -> type = LE;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "eq")) {
    theToken -> type = EQ;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "ge")) {
    theToken -> type = GE;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "gt")) {
    theToken -> type = GT;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "if")) {
    theToken -> type = IF;
    strcpy(theToken->str, instr);
  }else if (!strcmp(instr, "else")){
    theToken -> type = ELSE;
    strcpy(theToken->str, instr);
  } else if(!strcmp(instr, "endif")) {
    theToken -> type = ENDIF;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "drop")) {
    theToken -> type = DROP;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "dup")) {
    theToken -> type = DUP;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "swap")) {
    theToken -> type = SWAP;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "abs")) {
    theToken -> type = ABS;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "^")) {
    theToken -> type = POW;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "min")) {
    theToken -> type = MIN;
    strcpy(theToken->str, instr);
  }else if(!strcmp(instr, "avg")) {
    theToken -> type = AVG;
    strcpy(theToken->str, instr);
  }else if (instr[0] == '0' && instr[1] == 'x' ){
    for (int i = 2; i < length; i++){
      if(!isxdigit(instr[i])){
        printf("Invalid token\n");
        theToken -> type = BROKEN_TOKEN;
      }
    }
    if((theToken -> type) != BROKEN_TOKEN){
      theToken -> type = LITERAL;
      theToken -> literal_value = strtol(instr, NULL, 16);
      strcpy(theToken->str, instr);
    }
  } else if(instr[0] == '-' || isdigit(instr[0])){
    for (int i = 1; i < length; i++){
      if(!isdigit(instr[i])){
        printf("Invalid token\n");
        theToken -> type = BROKEN_TOKEN;
      }
    }
    if((theToken -> type) != BROKEN_TOKEN){
      theToken -> type = LITERAL;
      theToken -> literal_value = atoi(instr);
    }    
  }else if(length > 2) {
    if(instr[0] == 'a' && instr[1] == 'r' && instr[2] == 'g'){
      theToken -> type = ARG;
      if(length ==4 && isdigit(instr[3])){
        theToken -> arg_no = (int) instr[3] - 48;
      } else if(length ==5 && (isdigit(instr[3]) && isdigit(instr[4]))){
        char digits[2] = {instr[3], instr[4]};
        int argumento = atoi(digits);
        if(argumento > 20){
          printf("Invalid argument \n");
        } else{
          theToken -> arg_no = argumento;
          strcpy(theToken->str, instr);
        }
      }
    } else{
      if(!isalpha(instr[0])){
        printf("Illegal Character\n");
        theToken -> type = BROKEN_TOKEN;
      } else{
        for(int i = 1; i <length; i++){
          if(!(isalnum(instr[i]) || instr[i]=='_')){
            printf("Illegal name\n");
            theToken -> type = BROKEN_TOKEN;
          }
        }
        if((theToken -> type) != BROKEN_TOKEN){
          theToken -> type = IDENT;
          strcpy(theToken->str, instr);
        } else{
          printf("Definitely a broken token\n");
          theToken -> type = BROKEN_TOKEN;
        }  
      }
    } 
  }
}

void write_token(token *theToken, FILE* output, hash_struct* my_table){
  char outputLine[8000] = {0};
  char* prologue = "\tADD R6 R6 #-3\n\tSTR R7 R6 #1\n\tSTR R5 R6 #0\n\tADD R5 R6 #0\n";
  char* popStore = "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n";
  char* storeOutput = "\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n";
  char* argNumToString = malloc(1000 * sizeof(char));
  char* CMP = "\tCMP R1 R2\n"; 
  char* argFull = malloc(1000 * sizeof(char));
  char* branchNumber = malloc(250 * sizeof(char));
  char* branchLabel = malloc(250 * sizeof(char));
  char* endLabel = malloc(250 * sizeof(char));   
  char *ifNumberString = malloc(100 * sizeof(char)); 
  //hash_struct* my_table = initTable(100);    
  int value;
  if((theToken-> type != EMPTY) && (theToken -> type != BROKEN_TOKEN)){
    switch(theToken -> type){
      case DEFUN:
        printf("made it \n");
        defun = 1;
        break;
      case IDENT:
        if(defun != 0){
          strcat(outputLine, "\t\t .CODE\n");
          strcat(outputLine, "\t\t .FALIGN\n");
          strcat(outputLine, theToken->str);
          strcat(outputLine, "\n");
          strcat(outputLine, prologue);
          defun = 0;
          strcat(outputLine,  "\t;;;BODY;;;;\n");
          strcpy(routine, theToken->str);
          //printf("routine is %s and %s\n", theToken->str, routine);  
        } else{
          strcat(outputLine,"\tJSR " );
          strcat(outputLine, theToken-> str);
          strcat(outputLine, "\n");
          strcat(outputLine, "\tADD R6 R6 #-1\n");
        }
        break;
      case RETURN:
        strcat(outputLine , "\t;;;;EPILOGUE;;;;\n");
        strcat(outputLine, "\tLDR R7 R6 #0\n\tSTR R7 R5 #2\n\tADD R6 R5 #0\n\tLDR R5 R6 #0\n\tLDR R7 R6 #1\n\tADD R6 R6 #3\n\tJMPR R7\n");
        break;
      case PLUS:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tADD R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case MINUS:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tSUB R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case MUL:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tMUL R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case DIV:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tDIV R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case MOD:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tMOD R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case AND:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tAND R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case OR:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tOR R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case NOT:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tNOT R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case LT:
        strcat(outputLine, popStore);
	    strcat(outputLine , CMP);
        strcat(outputLine,"\tBRn ");
	    sprintf(branchNumber , "%d" , branches);
	    branches++;
	    strcpy(branchLabel , "LESS_");
	    strcat(branchLabel , branchNumber);  
		strcat(outputLine , branchLabel);
		strcat(outputLine , "\n");
			
        strcat(outputLine, "\tCONST R0 #0\n");
     	strcat(outputLine, "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
		strcat(outputLine, "\tJMP END_");
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
            
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
		strcat(outputLine ,"\tCONST R0 #1\n");
		strcat(outputLine , "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
            
		strcpy(endLabel , "END_");
		strcat(endLabel , "LESS_");
		strcat(endLabel , branchNumber);
        strcat(endLabel , "\n");
		strcat(outputLine , endLabel);            
        break;
      case LE:
        strcat(outputLine, popStore);
	    strcat(outputLine , CMP);
        strcat(outputLine,"\tBRnz ");
	    sprintf(branchNumber , "%d" , branches);
	    branches++;
	    strcpy(branchLabel , "LESE_");
	    strcat(branchLabel , branchNumber);  
		strcat(outputLine , branchLabel);
		strcat(outputLine , "\n");
			
        strcat(outputLine, "\tCONST R0 #0\n");
     	strcat(outputLine, "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
		strcat(outputLine, "\tJMP END_");
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
            
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
		strcat(outputLine ,"\tCONST R0 #1\n");
		strcat(outputLine , "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
            
		strcpy(endLabel , "END_");
		strcat(endLabel , "LESE_");
		strcat(endLabel , branchNumber);
        strcat(endLabel , "\n");
		strcat(outputLine , endLabel);            
        break; 
      case EQ:
        strcat(outputLine, popStore);
	    strcat(outputLine , CMP);
        strcat(outputLine,"\tBRz ");
	    sprintf(branchNumber , "%d" , branches);
	    branches++;
	    strcpy(branchLabel , "EQ_");
	    strcat(branchLabel , branchNumber);  
		strcat(outputLine , branchLabel);
		strcat(outputLine , "\n");
			
        strcat(outputLine, "\tCONST R0 #0\n");
     	strcat(outputLine, "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
		strcat(outputLine, "\tJMP END_");
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
            
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
		strcat(outputLine ,"\tCONST R0 #1\n");
		strcat(outputLine , "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
            
		strcpy(endLabel , "END_");
		strcat(endLabel , "EQ_");
		strcat(endLabel , branchNumber);
        strcat(endLabel , "\n");
		strcat(outputLine , endLabel);            
        break;
      case GE:
        strcat(outputLine, popStore);
	    strcat(outputLine , CMP);
        strcat(outputLine,"\tBRzp ");
	    sprintf(branchNumber , "%d" , branches);
	    branches++;
	    strcpy(branchLabel , "GREE_");
	    strcat(branchLabel , branchNumber);  
		strcat(outputLine , branchLabel);
		strcat(outputLine , "\n");
			
        strcat(outputLine, "\tCONST R0 #0\n");
     	strcat(outputLine, "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
		strcat(outputLine, "\tJMP END_");
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
            
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
		strcat(outputLine ,"\tCONST R0 #1\n");
		strcat(outputLine , "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
            
		strcpy(endLabel , "END_");
		strcat(endLabel , "GREE_");
		strcat(endLabel , branchNumber);
        strcat(endLabel , "\n");
		strcat(outputLine , endLabel);            
        break;            
      case GT:
        strcat(outputLine, popStore);
	    strcat(outputLine , CMP);
        strcat(outputLine,"\tBRp ");
	    sprintf(branchNumber , "%d" , branches);
	    branches++;
	    strcpy(branchLabel , "GREA_");
	    strcat(branchLabel , branchNumber);  
		strcat(outputLine , branchLabel);
		strcat(outputLine , "\n");
			
        strcat(outputLine, "\tCONST R0 #0\n");
     	strcat(outputLine, "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
		strcat(outputLine, "\tJMP END_");
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
            
		strcat(outputLine, branchLabel);
		strcat(outputLine , "\n");
		strcat(outputLine ,"\tCONST R0 #1\n");
		strcat(outputLine , "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n");
            
		strcpy(endLabel , "END_");
		strcat(endLabel , "GREA_");
		strcat(endLabel , branchNumber);
        strcat(endLabel , "\n");
		strcat(outputLine , endLabel);            
        break;  
      case IF:
        ifNum++;
        put(my_table, ifNum, finalIf);
        printEntries(my_table);    
        curr = ifNum;
        finalIf = ifNum;
        sprintf(ifNumberString, "%d", ifNum);            
        strcat(outputLine,"IF_");
        //printf("IF_%s\n",ifNumberString);    
        strcat(outputLine, routine);
        strcat(outputLine, ifNumberString);
        strcat(outputLine, "\n");
        strcat(outputLine, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n");
        strcat(outputLine, "\tCMPI R1 #0\n");
        strcat(outputLine , "\tBRnz ELSE_");
       //printf("BRnz ELSE_%s\n",ifNumberString);        
        strcat(outputLine, routine);
        strcat(outputLine, ifNumberString);
        strcat(outputLine, "\n");
        break;
      case ELSE:
        sprintf(ifNumberString, "%d", curr);   
	    strcat(outputLine , "\tBRnzp END_IF_");
        //printf("BRnzp END_IF_%s\n",ifNumberString);            
        strcat(outputLine, routine);
        strcat(outputLine, ifNumberString);
        strcat(outputLine, "\n");    
        strcat(outputLine , "ELSE_");  
        //printf("ELSE_%s\n",ifNumberString);            
        strcat(outputLine, routine);
        strcat(outputLine, ifNumberString);
        strcat(outputLine, "\n");     
        break;
      case ENDIF:
        sprintf(ifNumberString, "%d", curr);  
        //printf("curr could is %d\n\n", curr);    
        finalIf = get(my_table, curr) -> value;  
        printEntries(my_table);                            
        //        finalIf = (int)(get(my_table, finalIf) -> value);   
        removeEntry(my_table, curr); 
        //printf("curr %d finalif %d\n", curr, finalIf);    
        printEntries(my_table);                
        curr = finalIf;    
        //curr--;
      
        strcat(outputLine, "END_IF_");
        //printf("END_IF_%s\n",ifNumberString);                        
        strcat(outputLine, routine);
        strcat(outputLine, ifNumberString);
        strcat(outputLine, "\n"); 
        break;            
      case XOR:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tXOR R1 R1 R2\n");
        strcat(outputLine, storeOutput);
        break;
      case DROP:
        strcat(outputLine, "\tADD R6 R6 #1\n");
        break;
      case DUP:
        strcat(outputLine, "\tLDR R1 R6 #0\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
      case SWAP:
        strcat(outputLine, popStore);
        strcat(outputLine, "\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R2 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
      case AVG:
        strcat(outputLine, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n\tLDR R3 R6 #0\n\tADD R6 R6 #1\n");
        strcat(outputLine, "\tADD R1 R1 R2\n\tADD R1 R1 R3\n");
        strcat(outputLine, "\tSTR R4 R6 #0\n\tAND R4 R4 #0\n\tADD R4 R4 #3\n\tDIV R1 R1 R4\n");    
        strcat(outputLine, "\tLDR R4 R6 #0\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");
        break; 
      case ABS:
        BR++;
        char* makeAB = malloc(1000 * sizeof(char));
        strcpy(makeAB,"SHIFT_ADD");
        char d[11]; 
        sprintf(d,"%d", BR);    
        strcat(makeAB, d);    
        strcat(outputLine, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n");
        strcat(outputLine, "\tSTR R4 R6 #-1\n\tAND R4 R4 #0\n");
        strcat(outputLine, "\tCMPI R1 #0\t\n BRzp ");
        strcat(outputLine,makeAB);
        strcat(outputLine,"\n");            
        strcat(outputLine, "\tCONST R4 0xFF\n\tHICONST R4 0xFF\n");
        strcat(outputLine,makeAB);
        strcat(outputLine,"\n");             
        strcat(outputLine, "\tXOR R1 R1 R4\n\t SUB R1 R1 R4\n");
        strcat(outputLine, "\tLDR R4 R6 #-1\n\t STR R1 R6 #-1\n\t ADD R6 R6 #-1\n");
        free(makeAB);
        break;
      case POW:
        BR++;
        strcat(outputLine, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n\tADD R2 R2 #-1\n");
        char* makeBR = malloc(1000 * sizeof(char));
        strcpy(makeBR,"EXPO");
        char s[11]; 
        sprintf(s,"%d", BR);    
        strcat(makeBR, s);
        strcat(outputLine, "\n\tSTR R4 R6 #-1\n\tAND R4 R4 #0\n\tADD R4 R4 R1\n");    
        strcat(outputLine, makeBR);
        strcat(outputLine,"\tMUL R1 R1 R4\n\tADD R2 R2 #-1\n\tBRp ");
        strcat(outputLine,makeBR);
        strcat(outputLine,"\n");
        strcat(outputLine, "\tLDR R4 R6 #-1\n\tSTR R1 R6 #-1\n\t ADD R6 R6 #-1\n");
        free(makeBR);
        break;
      case MIN:
        BR++;
        strcat(outputLine, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n\tADD R2 R2 #-1\n");
        char* makeMI = malloc(1000 * sizeof(char));
        char* shortCirc = malloc(1000 * sizeof(char));    
        strcpy(makeMI,"MINI");
        strcpy(shortCirc, "ENDMIN");
        char q[11]; 
        sprintf(q,"%d", BR);    
        strcat(makeMI, q);
        strcat(shortCirc, q);    
        strcat(outputLine, "\tCMP R1 R2\n");
        strcat(outputLine, "\tBRnz ");    
        strcat(outputLine, makeMI);
        strcat(outputLine, "\n");
        strcat(outputLine,"\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");
        strcat(outputLine, "\tJMP ");
        strcat(outputLine, shortCirc);
        strcat(outputLine, "\n");    
        strcat(outputLine, makeMI);
        strcat(outputLine, "\n");
        strcat(outputLine,"\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");    
        strcat(outputLine, shortCirc);
        free(makeMI);
        free(shortCirc);    
        break;
      case ARG:
        sprintf(argNumToString, "%d", (theToken-> arg_no)+2);
        strcpy(argFull, "");    
        strcat(argFull, "\tLDR R3 R5 #");
        strcat(argFull, argNumToString);
        strcat(outputLine, argFull);
        strcat(outputLine, "\n");
        strcat(outputLine, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        break;
      case LITERAL:
        value = theToken -> literal_value;
        char* hiconst = malloc(1000 * sizeof(char));
        char* loconst = malloc(1000 * sizeof(char));
        char* value_str = malloc(1000 * sizeof(char)); 
        if(value > 256 || value < -256){
          int lBits = value & 0xFF;
          int hBits = (value >> 8) & 0xFF;
          sprintf(hiconst, "%d", hBits);
          sprintf(loconst, "%d", lBits);
          strcat(outputLine, "\tCONST R3 #");
          strcat(outputLine, loconst);
          strcat(outputLine, "\n");
          strcat(outputLine, "\tHICONST R3 #");
          strcat(outputLine, hiconst);
          strcat(outputLine, "\n");
          strcat(outputLine, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
        } else{
          sprintf(value_str, "%d", value);
          strcat(outputLine, "\tCONST R3 #");
          strcat(outputLine, value_str);
          strcat(outputLine, "\n");
          strcat(outputLine, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");          
        }
        free(value_str);
        free(hiconst);
        free(loconst);
        break;
      default:
        printf("Bruh!\n");
        printf("please dont give me points off I tried my best\n");
        break;
    }
  } else{
    printf("This is empty! YEET!\n wrongfil token with: ");
    print_token(theToken);
  }
  fwrite(outputLine, sizeof(char), strlen(outputLine), output);
  free(argFull);
  free(argNumToString);
  free(branchNumber);
  free(branchLabel);
  free(endLabel); 
  free(ifNumberString);  
  //done(my_table); 
}

//used for debugging
void print_token (token *theToken){
  printf("type: %u, literal value: %d, arg_no: %d, string: %s\n", theToken -> type, theToken->literal_value, theToken-> arg_no, theToken -> str);
}

