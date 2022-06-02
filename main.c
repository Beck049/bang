#include "card.h"

int main() {
	init_cards();
	for(int i = 0; i < card_num; ++i) {
		sCard* card = &cards[i];
		printf("%d\t", card->id);
		printf("%d\t", card->type);
		printf("%d\t", card->suit);
		printf("%d\t", card->num);
		printf("%s\t", card->name );
		printf("%s\n", card->description);
	}
	return 0;
}
