#include "list.h"

sListNode *new_node(void *data) {
	sListNode *pNode = calloc(1, sizeof(sListNode));
	pNode->data = data;
	pNode->prev = pNode;
	pNode->next = pNode;
	return pNode;
}

void free_node(sListNode *pNode) {
	free(pNode->data);
	free(pNode);
}

sList *new_list() {
	sList *pList = calloc(1, sizeof(sList));
	pList->end = new_node(NULL);
	return pList;
}

void list_insert(sList *pList, sListNode *pPos, sListNode *pNode) {
	++pList->size;
	pNode->prev = pPos->prev;
	pNode->prev->next = pNode;
	pNode->next = pPos;
	pNode->next->prev = pNode;
}

void list_erase(sList *pList, sListNode *pPos) {
	if(pPos == LIST_END(pList)) {
		fprintf(stderr, "error: erase end node in list: abort\n");
		abort();
	}
	--pList->size;
	pPos->prev->next = pPos->next;
	pPos->next->prev = pPos->prev;
	free_node(pPos);
}

void list_push_front(sList *pList, sListNode *pNode) {
	list_insert(pList, LIST_BEGIN(pList), pNode);
}

void list_push_back(sList *pList, sListNode *pNode) {
	list_insert(pList, LIST_END(pList), pNode);
}

void list_pop_front(sList *pList) {
	list_erase(pList, LIST_BEGIN(pList));
}

void list_pop_back(sList *pList) {
	list_erase(pList, LIST_END(pList)->prev);
}

i32 node_distance(sList *pList, sListNode *pNodeA, sListNode *pNodeB) {
	if(pNodeA == LIST_END(pList) || pNodeB == LIST_END(pList)) return -1;
	i32 res = 0;
	bool pass_end = false;
	while(pNodeA != pNodeB) {
		pNodeA = pNodeA->next;
		if(pNodeA != LIST_END(pList)) ++res;
		else pass_end = true;
	}
	return pass_end ? pList->size - res : res;
}


sListNode *node_advance(sList *pList, sListNode *pNode, i32 size) {
	while(size--) {
		pNode = pNode->next;
	}
	return pNode;
}

void list_clear(sList *pList) {
	while(pList->size > 0) {
		list_erase(pList, LIST_BEGIN(pList));
	}
}

void free_list(sList *pList) {
	list_clear(pList);
	free_node(LIST_END(pList));
	free(pList);
}
