int MAX_HEIGHT = 16;
Node* skiplist;
Node* update[MAX_HEIGHT];
srand(time(0));  //in main

Node* find(String word) {
	Node* node = head;

	for(h = height-1; h>0; --h) {
		while(node->next[h] != NULL && 
			node->next[h]->Entry.word < word) {
					//skip the words in between
					node = node->next[h];
		}

		if(node->next[h]->Entry.word.compare(word) == 0) {
		//if the node that node->next[h] points has an entry containing the same word as 
		//the input word  
			return node->next[h]; //found!
		}
		
		update[h] = node; //record the last pointer visited for every h
	}

	//entry not found!
	return NULL;
}

void insert(Node* node) {
	int height; //the height of the node (of the input entry) we need to insert
	for(i = 1; i<MAX_HEIGHT; ++i) 
		if (randBit()==1) break;

	height = i;
	Node* next[height];
	node->next = next;

	//only pointers in update array from index 0 to height need to be updated,
	//because the nodes visited at h>height do not change
	for(i=0; i<height-1; ++i) {
		next[i] = update[i];
		//this (to be inserted) node's next[i] contains a pointer
		//that points to what update[i] points to
		update[i] = node;
		//update[i] now points to this (to be inserted) node
	}	
}