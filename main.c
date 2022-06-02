#include "card.h"
#include "character.h"

int main() {
	init_cards();
	printf("cards:\n");
	for(int i = 0; i < card_num; ++i) {
		sCard* card = &cards[i];
		printf("%d\t", card->id);
		printf("%d\t", card->type);
		printf("%d\t", card->suit);
		printf("%d\t", card->num);
		printf("%s\t", card->name );
		printf("%s\n", card->description);
	}

	init_characters();
	printf("\ncharacters:\n");
	for(int i = 0; i < character_num; ++i) {
		sCharacter* character = &characters[i];
		printf("%d\t", character->id);
		printf("%s\n", character->name );
		printf("%s\n\n", character->description);
	}
	return 0;
}
