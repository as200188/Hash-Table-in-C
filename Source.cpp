/*
insert: key-value, the key should be unique.
find: return the key-value object in table.
remove: remove the table and release all memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <random>

static int TABLE_SIZE = 100;

struct KeyValue {
	int key; // unique
	int val;
};

struct Node {
	KeyValue *kv;
	Node *next;
};

struct HashTable {
	KeyValue* kv;
	Node* next;
};

int cmp(KeyValue* a, KeyValue* b);

int hash_func(int key) {
	return key % TABLE_SIZE;
}

bool insert(struct HashTable * hash_table, KeyValue *new_obj) {
	if (hash_table) {
		// get index of table.
		int index = hash_func(new_obj->key);

		if (hash_table[index].kv) {
			// Collision!!!
			// need to handle.
			// new a node and add to list.
			struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
			if (new_node) {
				new_node->kv = new_obj;
				new_node->next = NULL;

				// handle the collision.
				Node* old_node = hash_table[index].next;
				hash_table[index].next = new_node;
				new_node->next = old_node;

			}
		}
		else {
			// This space is empty.
			hash_table[index].kv = new_obj;
			return true;
		}
	}

	return false;
}

KeyValue *find(HashTable* hash_table, KeyValue *kv) {
	if (hash_table) {
		// get index of table.
		int index = hash_func(kv->key);

		if (hash_table[index].kv) {
			// Checking the object and another object in table are same.
			if (cmp(hash_table[index].kv, kv) == 0) {
				return hash_table[index].kv;
			}

			Node* temp = hash_table[index].next;
			while (temp) {
				if (cmp(temp->kv, kv) == 0)
					return hash_table[index].kv;
				temp = temp->next;
			}

		}
	}

	return NULL;
}

HashTable *build_table() {
	struct HashTable* hash_table = (struct HashTable*)malloc(TABLE_SIZE * sizeof(struct HashTable));

	if (hash_table) {
		// initial
		for (int i = 0; i < TABLE_SIZE; i++) {
			hash_table[i].kv = NULL;
			hash_table[i].next = NULL;
		}

		return hash_table;
	}

	return NULL;
}

void remove_table(HashTable* hash_table) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hash_table[i].kv) {
			free(hash_table[i].kv);
			Node* temp = hash_table[i].next;
			Node* del_node = NULL;
			while (temp) {
				del_node = temp;
				temp = temp->next;
				free(del_node);
			}
		}
	}
	free(hash_table);
	hash_table = NULL;
	return;
}

int cmp(KeyValue *a, KeyValue *b) {
	if (a->key == b->key)
		return 0;
	else if (a->key > b->key)
		return 1;
	else
		return -1;
}

int main() {

	HashTable* hash_table = build_table();

	for (int i = 0; i < 10000; i++) {
		KeyValue* kv = (KeyValue*)malloc(sizeof(KeyValue));
		if (kv) {
			// initial
			kv->key = i;
			kv->val = rand();
			insert(hash_table, kv);
		}
	}
	
	KeyValue* kv = (KeyValue*)malloc(sizeof(KeyValue));
	if (kv) {
		kv->key = 1000;
		kv->val = 15621;

		if (find(hash_table, kv))
			printf("%s", "find it.");
		else
			printf("%s", "Not found.");

	}

	remove_table(hash_table);

	return 0;
}