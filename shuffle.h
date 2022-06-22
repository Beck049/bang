#pragma once

#include "basic.h"
#include "list.h"

#define RANDOM_SORT(_arr, _size) { \
	srandom(time(NULL)); \
	i32 sort_time = random()%(_size)+(_size); \
	for(i32 i = 0; i < sort_time; ++i) { \
		qsort((_arr), (_size), sizeof(typeof(*(_arr))), random_cmp); \
	} \
} \

int random_cmp(__attribute__((unused)) const void *a, __attribute__((unused)) const void *b);

void list_init(sList *pList, i32 size);

void shuffle(sList *pList);