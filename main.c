#include <stdio.h>
#include <stdlib.h>
#include "Hash_Table.h"


int main(int argc, char* argv[])
{
    
    HashTable* hash_table = NULL;
    init_hash_table(&hash_table, 100);
    int i;
    for (i = 0; i < 1000; i++) {
		KeyValue* obj = malloc(sizeof(KeyValue));
		if (obj) {
			// initial
			obj->key = i;
			obj->val = 0;
			hash_table->insert(hash_table, obj);
		}
	}
	
	KeyValue* obj = (KeyValue*)malloc(sizeof(KeyValue));
	if (obj) {
		obj->key = 1;
		obj->val = 15621;
        printf("Removed Key: %d\n", obj->key);
        hash_table->remove(hash_table, obj);
        
        printf("Key: %d", obj->key);
		if (hash_table->find(hash_table, obj))
			printf("%s", " find it.\n");
		else
			printf("%s", " Not found.\n");
			
		obj->key = 101;
		obj->val = 21;	
		printf("Key: %d", obj->key);
        
		if (hash_table->find(hash_table, obj))
			printf("%s", " find it.\n");
		else
			printf("%s", " Not found.\n");
			
		obj->key = 901;
		obj->val = 21;	
		printf("Key: %d", obj->key);
        
		if (hash_table->find(hash_table, obj))
			printf("%s", " find it.\n");
		else
			printf("%s", " Not found.\n");
			
		obj->key = 1001;
		obj->val = 21;	
		printf("Key: %d", obj->key);
        
		if (hash_table->find(hash_table, obj))
			printf("%s", " find it.\n");
		else
			printf("%s", " Not found.\n");
			
		
	}
	
	// remove all table and release memory.
	hash_table->remove_all(hash_table);
    
    
    return 0;
}
