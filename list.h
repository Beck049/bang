#pragma once
#include "basic.h"

#define LIST_BEGIN(_pList) (_pList->end->next)
#define LIST_END(_pList) (_pList->end)
#define LIST_FRONT(_pList) (LIST_BEGIN(_pList)->data)
#define LIST_BACK(_pList) (LIST_END(_pList)->prev->data)
#define LIST_FOR_EACH(_pNode, _pList) for(sListNode *_pNode = LIST_BEGIN(_pList); _pNode != LIST_END(_pList); _pNode = _pNode->next)

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

void free_list(sList *pList) {
	while(pList->size != 0) {
		list_erase(pList, LIST_BEGIN(pList));
	}
	free_node(LIST_END(pList));
	free(pList);
}
