#pragma once
#include "basic.h"

#define LIST_FOR_EACH(_cur, _pList) for(sListNode *_cur = pList->end->next; _cur != pList->end; cur = _cur->next)

typedef struct _sListNode {
	void *data;
	struct _sListNode *prev, *next;
} sListNode;

typedef struct _sList {
	sListNode *end;
	size_t size;
} sList;

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

void list_insert(sListNode *pPos, sListNode *pNode) {
	pNode->prev = pPos->prev;
	pNode->prev->next = pNode;
	pNode->next = pPos;
	pNode->next->prev = pNode;
}

void list_erase(sListNode *pPos) {
	pPos->prev->next = pPos->next;
	pPos->next->prev = pPos->prev;
	free_node(pPos);
}

void list_push_back(sList *pList, sListNode *pNode) {
	list_insert(pList->end, pNode);
}

void free_list(sList *pList) {
	sListNode **cur = &pList->end->prev;
	while(*cur != pList->end) {
		list_erase(*cur);
	}
	free_node(*cur);
	free(pList);
}
