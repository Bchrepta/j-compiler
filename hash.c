#include "hash.h"


/**
* @input - A number of buckets, the size to make the hash table.
* Should assign space for the hash_struct pointer, all buckets, and should
* initialize all entries as KEY = -1, VALUE = -1 to begin.
* 
*/
int finder;
hash_struct* initTable(int num_of_buckets){
  int i =0;
  hash_struct* hash_table = NULL;
  hash_table = malloc(sizeof(hash_struct)); //check?
  hash_table -> buckets = malloc(sizeof(data_entry*)*num_of_buckets);
  hash_table -> num_of_buckets = num_of_buckets;

  while (i< num_of_buckets){

    hash_table->buckets[i] = malloc(sizeof(data_entry));
    hash_table->buckets[i]-> key = -1;
    hash_table->buckets[i]-> value = -1;
    
    i++;
  }

  return hash_table;


}


/*
* @input : Some key value.
* @returns : The key value modulated by the size of the hash table.
*/

int hashcode(hash_struct* table, int key){
  int newKey;
  if(key <0){
    newKey = (table -> num_of_buckets)+((key));
  } else{
    newKey = key;
  }
  return newKey % (table -> num_of_buckets);
}

/*
* @input : Some key value, table pointer.
* @returns : The data entry if some matching (key, value) pair exists, otherwise returns NULL.
*/
data_entry* get(hash_struct* table, int key){ //(-1,5) and (5, -1) are both valid ?
  if(key ==-1){
    for(int i = 0; i < (table -> num_of_buckets); i++){
      if(((table->buckets[hashcode(table, i)] -> key) == key) && ((table->buckets[hashcode(table, i)] -> value) != -1)){
        finder = i;
        return (table->buckets[hashcode(table, i)]);
      }
    }    
  }else{
    for(int i = 0; i < (table -> num_of_buckets); i++){
      if((table->buckets[hashcode(table, i)] -> key) == key){
        finder = i;
        return ((table->buckets[hashcode(table, i)]) );
      }
    }
  } 
  return NULL;
}

/*
* @input : Some key value, table pointer.
* @returns : True only if a valid key exists that maps to this value.
*/
bool contains(hash_struct* table, int key){
  if(get(table, key) !=0 ){
    return true;
  } else{
    return false;
  }

}

/*
* @input : Some key integer.
* @input : Some value,.
* @returns : void. Places (key, value) pairing into map.
* Replaces value if another pairing with this key already exists.
* Do nothing if the table is full!
*/
void put(hash_struct* table, int key, int value){
  int i = key;
  bool state = true;
  if(contains(table, key)){
    //free(table->buckets[hashcode(table, key)]);
    //table->buckets[hashcode(table, key)]= malloc(sizeof(data_entry));
    table->buckets[hashcode(table, key)] -> key = key;
    table->buckets[hashcode(table, key)] -> value = value;
  } else{
    if(!(isFull(table))){
      do{
        if((table->buckets[hashcode(table, key)] -> key) == -1 && (table->buckets[hashcode(table, key)] -> value) == -1){
          //free(table->buckets[hashcode(table, key)]);
          //table->buckets[hashcode(table, key)]= malloc(sizeof(data_entry));
          table->buckets[hashcode(table, key)] -> key = key;
          table->buckets[hashcode(table, key)] -> value = value; 
          state = false;
          break;
        } else{
          i++;
          if((table->buckets[hashcode(table, i)] -> key) == -1 && (table->buckets[hashcode(table, i)] -> value) == -1){
            //free(table->buckets[hashcode(table, i)]);
            //table->buckets[hashcode(table, i)]= malloc(sizeof(data_entry));
            table->buckets[hashcode(table, i)] -> key = key;
            table->buckets[hashcode(table, i)] -> value = value;            
            state = false;
          } else{
            //nothing
          }
        }
      } while (state);
    }
  }
}

/*
* @returns : The number of valid (key, value) pairings in the table.
*/
int size(hash_struct* table){
  int tableSize = 0;
  for(int i = 0; i < (table -> num_of_buckets); i++){
    if((table->buckets[hashcode(table, i)] -> key) != -1 || ((table->buckets[hashcode(table, i)] -> value) != -1)){
      tableSize++;
    }
  
  }
  return tableSize;
}

/*
* Iterates through the table and resets all entries.
*/
void clear(hash_struct* table){
  int i =0;
  while (i < table->num_of_buckets){
    table->buckets[hashcode(table, i)] -> key = -1;
    table->buckets[hashcode(table, i)]-> value = -1;
    i++;
  }
}

/*
* @returns : true, only if the table contains 0 valid (key, value) pairings.
*/
bool isEmpty(hash_struct* table){
     return (size(table)==0);

}


/*
* @returns : true, only when the table is filled entirely with VALID values.
*/
bool isFull(hash_struct* table){

       return (size(table) == table-> num_of_buckets);
}


/*
* @input : Some key value.
* @returns : void. if a pair exists for this key, reinitialize this entry.
*/
void removeEntry(hash_struct* table, int key){
  if(contains(table, key)){
    table->buckets[hashcode(table, finder)] -> key = -1;    
    table->buckets[hashcode(table, finder)] -> value = -1;    
  }
  printf("New size %d\n", size(table));
}


/*
* Debugging function.
* Iterates through the hashTable and prints all NON-NULL (key, value) pairings.
* Print statement should be of the form "(key1, value1), (key2, value2), ...."
*/
void printEntries(hash_struct* table){
  int i = 0;
  while(i< (table-> num_of_buckets)){
    if(!((table->buckets[hashcode(table, i)] -> key) == -1 && (table->buckets[hashcode(table, i)] -> value) == -1)){
    printf("(%d, %d), ", (table->buckets[hashcode(table, i)] -> key), (table->buckets[hashcode(table, i)] -> value));
    }
    i++;
  }
  printf("END\n");

}

/*
* Debugging function.
* Iterates though the hashTable and prints ALL entries in order.
* If a value is not valid, you will print "EMPTY" for the value.
* Entry print Format: "(INDEX: tableIndex, KEY: key, VALUE: value)"
* Example :  "(INDEX: 0, KEY: 0, VALUE: 3), (INDEX: 1, KEY: EMPTY, VALUE: EMPTY), (INDEX: 2, KEY: 2, VALUE: 49), ..."
*/
void printFullTable(hash_struct* table){

                                      
  for(int i=0; i < (table-> num_of_buckets); i++){
    if((table->buckets[i%(table->num_of_buckets)] -> key) == -1 && (table->buckets[i%(table->num_of_buckets)] -> value) == -1 ){

      printf("INDEX: %d, KEY: EMPTY, VALUE: EMPTY\n", i);
    } else{
      int aKey = (table->buckets[i%(table->num_of_buckets)] -> key);
      int aValue = (table->buckets[i%(table->num_of_buckets)] -> value);
      printf("INDEX: %d, KEY: %d, VALUE: %d\n", i, aKey, aValue);
    }
  }
}


/**
* Should free all space consumed by the hash table.
*/
void done(hash_struct* table){
  for(int i =0; i< table -> num_of_buckets; i++){
    free(table -> buckets[i]); //invalid sometimes?
  }
  free(table -> buckets);
  free(table);
}