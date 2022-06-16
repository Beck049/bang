#include "display.h"

void display_pile(sList *pList) {
	static const char SUIT[] = "SHDC";
	i32 row = 1;
	LIST_FOR_EACH(pNode, pList) {
		i32 card_id = *(i32*)pNode->data;
		sCard *card = cards+card_id;
		printf("%2d) %c%-2d %s: %s\n", row, SUIT[card->suit], card->num, card->name, card->description);
		++row;
	}
}