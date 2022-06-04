#include "card.h"
#include "character.h"
#include "list.h"

int main() {
	// init_cards();
	// printf("cards:\n");
	// for(int i = 0; i < card_num; ++i) {
	// 	sCard* card = &cards[i];
	// 	printf("%d\t", card->id);
	// 	printf("%d\t", card->type);
	// 	printf("%d\t", card->suit);
	// 	printf("%d\t", card->num);
	// 	printf("%s\t", card->name );
	// 	printf("%s\n", card->description);
	// }
	// init_characters();
	// printf("\ncharacters:\n");
	// for(int i = 0; i < character_num; ++i) {
	// 	sCharacter* character = &characters[i];
	// 	printf("%d\t", character->id);
	// 	printf("%s\n", character->name );
	// 	printf("%s\n\n", character->description);
	// }
	
	char buf[32];

	sList *pList = new_list();

	for(i32 i = 0; i < 100; ++i) {
		sprintf(buf, "%d", i*2-3);
		sListNode *pNode = new_node(strdup(buf));
		list_push_back(pList, pNode);
	}

	LIST_FOR_EACH(cur, pList) {
		printf("%s\n", (char*)cur->data);
	}

	free_list(pList);
	return 0;
}
