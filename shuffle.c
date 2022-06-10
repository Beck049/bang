#include "shuffle.h"

int random_cmp(__attribute__((unused)) const void *a, __attribute__((unused)) const void *b) {
	i32 val = random()%3 - 1;  // 0 1 2 -> -1 0 +1
	return val;
}

void list_init(sList *pList, i32 size) {
	for(i32 i = 0; i < size; ++i) {
		i32 *id = malloc(sizeof(i32));
		*id = i;
		list_push_back(pList, new_node(id));
	}
}

void shuffle(sList *pList) {
	if(pList->size == 0) return;

	void *arr[pList->size];

	i32 cnt = 0;
	LIST_FOR_EACH(pNode, pList) {
		arr[cnt++] = pNode->data;
	}

	RANDOM_SORT(arr, pList->size);
	
	cnt = 0;
	LIST_FOR_EACH(pNode, pList) {
		pNode->data = arr[cnt++];
	}
}