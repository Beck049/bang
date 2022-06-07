#pragma once

#include "basic.h"
#include "list.h"


void *draw(sList *pList) {
    void *ret = LIST_FRONT(pList);
	list_pop_front(pList);
    return ret;
}
