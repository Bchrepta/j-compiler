#include "hash.h"

#include <stdbool.h>
#include <assert.h>

/**
 * This file is to check list.h and list.c for part I of the assignment only
 *
 * Do not include it in the submission of part II
 *
 * You are expected to add additional test cases to this file
 * and to run valgrind on the program once you have it working
 */

//Tests that the hashcode function works
bool basicHashcode(hash_struct* table){


    if (hashcode(table, 7) == 7 && hashcode(table, 107) == 7){
        return true;
    } else {
        return false;
    }

}


//tests that an empty list has size == 0
bool testBasicEmpty(hash_struct* table){

    clear(table);

    if (isEmpty(table) == true){
        return true;
    } return false;

}

bool testPut(hash_struct* table){


    if (isEmpty(table) == false){
        return true;
    } return false;

}

bool testFull(hash_struct* table){


    if (isFull(table) == true){
        return true;
    } return false;

}


int main()
{
    printf ("Creating table\n") ;

    hash_struct* my_table = initTable(100) ;
    hash_struct* my_gable = initTable(10) ;
  
    if (basicHashcode(my_table) == true){
         printf("%s\n", "Passed Test 1 : basicHashcode()");
    } else {
        printf("%s\n", "Failed Test 1 : basicHashcode()");
    }
  put(my_gable, 1, 3);
  put(my_gable, 2, 3);
  put(my_gable, 3, 3);
  put(my_gable, 4, 3);
  put(my_gable, 5, 3);
  put(my_gable, 7, 3);
  put(my_gable, 6, 3);
  put(my_gable, 8, 3);
  put(my_gable, 9, 3);
  put(my_gable, 550, 49);
  put(my_gable, 10, 3);
  printEntries(my_gable);
  printf("is it full %d\n", isFull(my_gable));
  printf("Attmpting removeal of 1, should contain \n");
  removeEntry(my_gable, 1);
  printf("is it full %d\n", isFull(my_gable));
  printf("size %d\n", size(my_gable));
  printEntries(my_gable);
  put(my_gable, 1, 16);
  removeEntry(my_gable, 5345);
  printf(" 1 size %d\n", size(my_gable));
  printEntries(my_gable);
  removeEntry(my_gable, 9);
  printf("2 size %d\n", size(my_gable));
  printEntries(my_gable);
    if (testBasicEmpty(my_table) == true){
        printf("%s\n", "Passed Test 2 : testBasicEmpty()");
    } else {
        printf("%s\n", "Failed Test 2 : testBasicEmpty()");
    }


    //-------------------YOU CAN ADD YOUR OWN TESTS HERE---------



    //-------------------------------------------------



    printf ("freeing table\n") ;
    done(my_table) ;
    done(my_gable);


    return 0;

}
