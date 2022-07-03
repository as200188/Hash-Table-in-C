#ifndef Hash_Table
#define Hash_Table

/*
insert: key-value, the key should be unique.
find: return the key-value object in table.
remove_all: remove the table and release all memory.
*/

/* forward declaration */
/* data structure define */ 
typedef struct key_value_t KeyValue;
typedef struct node_t Node;
typedef struct object HashTable;
/* function define */
typedef int cmp_f(KeyValue*, KeyValue*);
typedef int hash_f(HashTable*, KeyValue*);
typedef int insert_f(HashTable*, KeyValue*);
typedef KeyValue* find_f(HashTable*, KeyValue*);
typedef void remove_all_f(HashTable*);
typedef void remove_f(HashTable*, KeyValue*);


struct key_value_t {
    int key; //Key should be unique.
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
    remove_f *remove;
};

static void remove_impl(HashTable* self, KeyValue *search_obj){
    
    // Get index of table.
	int index = self->hash_func(self, search_obj);
	
    if (self->table[index].obj) {
        Node *pre, *head, *cur, *next = NULL;
        head = &(self->table[index]);
        cur = head;
        next = head->next;
		// Compare keys.
		// Head.
		if (self->cmp(self->table[index].obj, search_obj) == 0) {
			free(head->obj);
			head->obj = next->obj;
			head->next = next->next;
			free(next);
		}
		else{
		    // Not found, try to search linked list.
    		// This entry may exist linked list.
    		// Search this linked list.
    		pre = cur;
    		cur = next;
    		next = cur->next;
    		
    		while (cur) {
    			if (self->cmp(cur->obj, search_obj) == 0){
    			    pre->next = cur->next;
    			    free(cur->obj);
    			    free(cur);
    			    return;
                }
                
    			pre = cur;
        		cur = next;
        		next = cur->next;
    		}
        }
	}
    
    return;

}

static void remove_all_impl(HashTable *self) {
    // Remove all node and release memory space.
   int i;
   for (i = 0; i < self->TABLE_SIZE; i++) {
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
	
	// Get index of table.
	int index = self->hash_func(self, search_obj);

	if (self->table[index].obj) {
		// Compare keys.
		if (self->cmp(self->table[index].obj, search_obj) == 0) {
			return self->table[index].obj;
		}
		
		// Not found, try to search linked list.
		Node* temp = self->table[index].next;
		// This entry may exist linked list.
		// Search this linked list.
		while (temp) {
			if (self->cmp(temp->obj, search_obj) == 0)
				return self->table[index].obj;
			temp = temp->next;
		}
	}

	return NULL; // Can't find.
}

static int insert_impl(HashTable *self, KeyValue *new_obj) { // method
	if(!self)
		return 0;

	// Get index of table.
	int index = self->hash_func(self, new_obj);

	if (self->table[index].obj) {
		// Collision!!!
		// Handle this situation.
		// New a node and add to list.
		Node* new_node = malloc(sizeof(Node));
		if (new_node) {
			new_node->obj = new_obj;
			new_node->next = NULL;

			// Add linked list.
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
    // Compute value of index.
    return obj->key % self->TABLE_SIZE;
}

static int cmp_impl(KeyValue *a, KeyValue *b) { // method
    // This method can be replaced by assigning new implementation of function.
    if(!(a && b)){
        return -1;
    }
    
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
    
    // Dynamically allocate memory.
    (*self)->table = malloc(table_size * sizeof(Node));
    // Dynamically allocate memory may be failed.
    if(!(*self)->table)
		return 0;
	
	// Initial entire table.
	int i;
	for(i = 0; i < table_size; i++) {
		(*self)->table[i].obj = NULL;
		(*self)->table[i].next = NULL;
	}
	
	// Assign implementation to function pointer.
	(*self)->TABLE_SIZE = table_size;
        (*self)->hash_func = hash_func_impl;
	(*self)->cmp = cmp_impl;
	(*self)->insert = insert_impl;
	(*self)->find = find_impl;
	(*self)->remove_all = remove_all_impl;
	(*self)->remove = remove_impl;
	
    return 0;
}

#endif
