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

sListNode *new_node(void *data);

void free_node(sListNode *pNode);

sList *new_list();

void list_insert(sList *pList, sListNode *pPos, sListNode *pNode);

void list_erase(sList *pList, sListNode *pPos);

void list_push_front(sList *pList, sListNode *pNode);

void list_push_back(sList *pList, sListNode *pNode);

void list_pop_front(sList *pList);

void list_pop_back(sList *pList);

i32 node_distance(sList *pList, sListNode *pNodeA, sListNode *pNodeB);

sListNode *node_advance(sList *pList, sListNode *pNode, i32 size);

void list_clear(sList *pList);

void free_list(sList *pList);
