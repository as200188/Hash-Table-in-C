/*
insert: key-value, the key should be unique.
find: return the key-value object in table.
remove_all: remove the table and release all memory.
*/

#include <stdio.h>
#include <stdlib.h>

/* forward declaration */
typedef struct key_value_t KeyValue;
typedef struct node_t Node;
typedef struct object HashTable;
typedef int cmp_f(KeyValue*, KeyValue*);
typedef int hash_f(HashTable*, KeyValue*);
typedef int insert_f(HashTable*, KeyValue*);
typedef KeyValue* find_f(HashTable*, KeyValue *);
typedef void remove_all_f(HashTable*);


struct key_value_t {
    int key; // should be unique
    int val;
};

struct node_t {
	KeyValue *obj;
	Node *next;
};

struct object {
    Node *table;
    int TABLE_SIZE;
    cmp_f *cmp;
    hash_f *hash_func;
    insert_f *insert;
    find_f *find;
    remove_all_f *remove_all;
};

static void remove_all_impl(HashTable* self) {
	for (int i = 0; i < self->TABLE_SIZE; i++) {
		if (self->table[i].obj) {
			free(self->table[i].obj);
			Node* temp = self->table[i].next;
			Node* del_node = NULL;
			while (temp) {
				del_node = temp;
				temp = temp->next;
				free(del_node);
			}
		}
	}
	free(self);
	self = NULL;
	return;
}

static KeyValue* find_impl(HashTable *self, KeyValue *search_obj) { // method
	if(!self)
		return NULL;
	
	// get index of table.
	int index = self->hash_func(self, search_obj);

	if (self->table[index].obj) {
		// Checking the object and another object in table are same.
		if (self->cmp(self->table[index].obj, search_obj) == 0) {
			return self->table[index].obj;
		}

		Node* temp = self->table[index].next;
		while (temp) {
			if (self->cmp(temp->obj, search_obj) == 0)
				return self->table[index].obj;
			temp = temp->next;
		}

	}

	return NULL;
}

static int insert_impl(HashTable *self, KeyValue *new_obj) { // method
	if(!self)
		return 0;

	// get index of table.
	int index = self->hash_func(self, new_obj);

	if (self->table[index].obj) {
		// Collision!!!
		// need to handle.
		// new a node and add to list.
		Node* new_node = malloc(sizeof(Node));
		if (new_node) {
			new_node->obj = new_obj;
			new_node->next = NULL;

			// handle the collision.
			Node* old_node = self->table[index].next;
			self->table[index].next = new_node;
			new_node->next = old_node;
			return 1;
		}
	}
	else {
		// This space is empty.
		self->table[index].obj = new_obj;
		return 1;
	}

	return 0;
}

static int hash_func_impl(HashTable *self, KeyValue *obj) { // method
    return obj->key % self->TABLE_SIZE;
}

static int cmp_impl(KeyValue *a, KeyValue *b) { // method
    if (a->key == b->key)
		return 0;
	else if (a->key > b->key)
		return 1;
	else
		return -1;
}

// & : address of
// * : value of // indirect access
int init_hash_table(HashTable** self, int table_size) { // call-by-value
	*self = malloc(sizeof(HashTable));
    if (!(*self))
		return 0;
    
    (*self)->table = malloc(table_size * sizeof(Node));
    if(!(*self)->table)
		return 0;
	
	for(int i = 0; i < table_size; i++) {
		(*self)->table[i].obj = NULL;
		(*self)->table[i].next = NULL;
	}
	
	
	(*self)->TABLE_SIZE = table_size;
    (*self)->hash_func = hash_func_impl;
	(*self)->cmp = cmp_impl;
	(*self)->insert = insert_impl;
	(*self)->find = find_impl;
	(*self)->remove_all = remove_all_impl;
    return 0;
}

int main(int argc, char* argv[])
{
    
    HashTable* hash_table = NULL;
    init_hash_table(&hash_table, 100);
    for (int i = 0; i < 1000; i++) {
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
		obj->key = 100;
		obj->val = 15621;

		if (hash_table->find(hash_table, obj))
			printf("%s", "find it.");
		else
			printf("%s", "Not found.");
	}
	
	// remove all table and release memory.
	hash_table->remove_all(hash_table);
    
    
    return 0;
}
