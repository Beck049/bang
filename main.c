// #include "init_card.h"
// #include "init_character.h"
#include "event.h"
#include "select.h"
#include "draw.h"
#include "display.h"
#include "determine.h"
#include "bang.h"


int main() {
	init_cards();
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
	

	// sList *pList = new_list();
	// for(i32 i = 0; i < card_num; ++i) {
	// 	i32 *data = malloc(sizeof(i32));
	// 	*data = i;
	// 	list_push_back(pList, new_node(data));
	// }
	// LIST_FOR_EACH(pNode, pList) {
	// 	printf("%d\n", *(i32*)pNode->data);
	// }
	// display_pile(pList);
	// free_list(pList);

	init_event_funcs();

	// list_push_back(select_event_funcs[0], {void*a = NULL; a;});
	
	// select_event_player();

	sGame *pGame = new_game();
	for(i32 i = 0; i < 38; ++i) {
		give_card(pGame, pGame->draw_pile, i, true);
	}
	// shuffle(pGame->draw_pile);
	// give_card(pGame, pGame->draw_pile, 1, true);
	display_game(pGame, 0);
	sSelectEvent sl_e = {
		.min_cnt = 3,
		.max_cnt = 5,
		.selections = new_list(),
		.select_res = new_list()
	};
	for(i32 i = 0; i < 5; ++i) {
		char tmp[128];
		sprintf(tmp, "%d) %d", i+1, i*3+2);
		list_push_back(sl_e.selections, new_node(strdup(tmp)));
	}
	select_event_bot(pGame, &sl_e);
	// select_event_player(pGame, &sl_e);
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	// take_card(pGame, pGame->draw_pile, 0);

	// for(i32 i = 0; i < 38; ++i) {
	// 	sDetermineEvent e;
	// 	e.target_id = 0;
	// 	determine_event_default(pGame, &e);
	// 	printf("res: %d\n", e.determine_res);
	// }
	display_end(NULL, Sheriff);
	display_end(NULL, Outlaws);
	display_end(NULL, Renegade);
	
	free_game(pGame);

	free_event_funcs();
	return 0;
}
