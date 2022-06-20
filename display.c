#include "display.h"


static const char SUIT[] = "SHDC";
// static const char *EMPTY_CARD_FRAME = " +             + ";
// static const char* EMPTY_DESK_FRAME = "||                                                                            ||";

size_t strlen_fix(const char *str) {
	const char *cur = str;
	size_t len = 0;
	while(*cur != '\0') {
		if((uint8_t)*cur > 0x7Fu) {
			len += 2;
			cur += 3;
		}
		else {
			len += 1;
			cur += 1;
		}
	}
	// printf("len of '%s' is %ld\n", str, len);
	return len;
}

void print_card(char *dst, i32 card_id) {
	sCard *card = cards+card_id;
	sprintf(dst, "%c%-2d %s: %s", SUIT[card->suit], card->num, card->name, card->description);
}

void print_center(char *dst, const char *src) {
	char buf[strlen(dst)+strlen(src)];
	memset(buf, 0, sizeof(buf));

	i32 width = strlen(dst);
	i32 len_fix = strlen_fix(src);
	i32 l_padding = (width-len_fix)/2;
	i32 r_padding = (width-len_fix)-l_padding;

	char *cur = buf;
	memcpy(cur, dst, l_padding);
	cur += l_padding;
	memcpy(cur, src, strlen(src));
	cur += strlen(src);
	memcpy(cur, dst+width-r_padding, r_padding);
	cur += r_padding;
	*cur = '\0';
	strcpy(dst, buf);
}

void print_with_frame(char *dst, const char *src, const char *frame) {
	strcpy(dst, frame);
	print_center(dst, src);
}

void display_pile(sList *pList) {
	char buf[BUFSIZ];
	i32 row = 1;
	LIST_FOR_EACH(pNode, pList) {
		i32 card_id = *(i32*)pNode->data;
		print_card(buf, card_id);
		printf("%2d) %s\n", row, buf+4);
		++row;
	}
}

void display_board(sGame *pGame, i32 viewer_id) {
	// reset position
	printf("\033[2J\033[1;1H");
	printf("++============================================================================++\n");
	printf("||                                                                            ||\n");
	printf("||                 +----------------- BANG -----------------+                 ||\n");
	printf("||                 | Draw Pile (%2ld)    /  Discard Pile (%2ld) |                 ||\n", pGame->draw_pile->size, pGame->discard_pile->size);
	printf("||                 +----------------------------------------+                 ||\n");

	sList *live_player = pGame->live_players;

	sListNode *player_node = get_player(pGame, viewer_id);
	if(player_node == NULL) {
		player_node = LIST_BEGIN(live_player);
	}
	
	// loop every player
	i32 live_size = live_player->size;
	for(i32 i = 0; i < live_size; ++i) {
		i32 player_id = *(i32*)player_node->data;
		sList *player_cards = pGame->players[player_id].cards;
		sList *player_desk = pGame->players[player_id].desk;
		sListNode *pCurNode;

		// build appellation
		char appellation[16];
		if(player_id == viewer_id) sprintf(appellation, "  你  ");
		else                       sprintf(appellation, "玩家 %d", player_id);

		printf("||                                                                            ||\n");
		printf("||     [ %s ]  hp:%2d  Cards:%2ld                                            ||\n", appellation, pGame->players[player_id].hp, player_cards->size);
		printf("||                                                                            ||\n");

		i32 desk_size = (i32)player_desk->size;

		pCurNode = LIST_BEGIN(player_desk);
		for(i32 j = 0; j < desk_size; j += 4) {
			char lines[4][4][64];
			// k is column
			for(i32 k = 0; k < 4; ++k) {
				i32 pos = j*4+k;
				if(pos == desk_size) break;
				i32 card_id = *(i32*)pCurNode->data;

				strcpy(lines[0][k],                                  " +-------------+ ");
				print_with_frame(lines[1][pos], cards[card_id].name, " |             | ");
				strcpy(lines[2][k],                                  " +-------------+ ");
				strcpy(lines[3][k],                                  "                 ");

				pCurNode = pCurNode->next;
			}

			// l is line
			for(i32 l = 0; l < 4; ++l) {
				printf("||    ");
				// k is column
				for(i32 k = 0; k < 4; ++k) {
					i32 pos = j*4+k;
					if(j+k < desk_size) printf("%s", lines[l][pos]);
					else                printf("                 ");
				}
				printf("    ||\n");
			}
		}
		printf("||                                                                            ||\n");
		player_node = get_next_player(pGame, player_node);
	}
	printf("||                                                                            ||\n");
	printf("++============================================================================++\n\n");
}

void display_hands(sGame *pGame, i32 viewer_id) {
	sList *viewer_hands = pGame->players[viewer_id].cards;
	// display_pile(viewer_hands);
	// printf("\n");

	if(viewer_id != 0) return;

	i32 hand_size = (i32)viewer_hands->size;

	printf("你的手牌: \n");
	if(hand_size == 0) {
		printf("Empty\n\n");
		return;
	}
	

	char lines[7][hand_size][64];
	sListNode *pViewerCardNode = LIST_BEGIN(viewer_hands);
	for(i32 i = 0; i < hand_size; ++i) {
		i32 card_id = *(i32*)pViewerCardNode->data;
		pViewerCardNode = pViewerCardNode->next;

		sCard *card = &cards[card_id];
		char suit_num_and_id[16];
		sprintf(suit_num_and_id, "%c%-2d      %2d", SUIT[card->suit], card->num, card_id);

		strcpy(lines[0][i],                            " +-------------+ ");
		print_with_frame(lines[1][i], suit_num_and_id, " +             + ");
		strcpy(lines[2][i],                            " +             + ");
		print_with_frame(lines[3][i], card->name,      " +             + ");
		strcpy(lines[4][i],                            " +             + ");
		strcpy(lines[5][i],                            " +             + ");
		strcpy(lines[6][i],                            " +-------------+ ");
	}
	for(i32 i = 0; i < 7; ++i) {
		for(i32 j = 0; j < hand_size; ++j) {
			printf("%s", lines[i][j]);
		}
		printf("\n");
	}
	// struct timespec ts_req = {.tv_nsec = 0, .tv_sec = 1};
	// nanosleep(&ts_req, NULL);
	printf("\n\n");
}

void display_damage(__attribute__((unused)) sGame *pGame, i32 viewer_id, sDamageEvent e) {
	char victim_appellation[16], damager_appellation[16];
	if(e.victim_id == viewer_id) {
		sprintf(victim_appellation, "You");
	}
	else {
		sprintf(victim_appellation, "Player%d", e.victim_id);
	}
	if(e.damager_id == viewer_id) {
		sprintf(damager_appellation, "you");
	}
	else {
		sprintf(damager_appellation, "player%d", e.damager_id);
	}
	if(e.damager_id != -1) printf("%s take %d damages by %s!\n", victim_appellation, e.damage, damager_appellation);
	else printf("%s take %d damages!\n", victim_appellation, e.damage);
}

void display_has_bomb(__attribute__((unused)) sGame *pGame, i32 viewer_id, i32 bomb_owner_id) {
	if(bomb_owner_id == viewer_id) {
		printf("You have bomb!\n");
	}
	else {
		printf("Player%d has bomb!\n", bomb_owner_id);
	}
}

void display_determine(__attribute__((unused)) sGame *pGame,__attribute__((unused)) i32 viewer_id, i32 draw_num) {
	if(draw_num == 0) return;
	printf("Determine Event:\n"
			"Draw %d card%c:\n", draw_num, "s"[draw_num == 1]);
}

void display_bang(__attribute__((unused)) sGame *pGame, i32 viewer_id, sBangEvent e) {
	char trigger_appellation[16], target_appellation[16];
	if(e.trigger_id == viewer_id) {
		sprintf(trigger_appellation, "You");
	}
	else {
		sprintf(trigger_appellation, "Player%d", e.trigger_id);
	}
	if(e.target_id == viewer_id) {
		sprintf(target_appellation, "you");
	}
	else {
		sprintf(target_appellation, "player%d", e.target_id);
	}
	printf("%s bang %s!\n", trigger_appellation, target_appellation);
}

void display_selection(__attribute__((unused)) sGame *pGame, __attribute__((unused)) i32 viewer_id, sSelectEvent e) {
	// printf("Select Event:\n");

	if(e.min_cnt < e.max_cnt) {
		printf("請選擇 %d ~ %d 個選項:\n", e.min_cnt, e.max_cnt);
	} else {
		printf("請選擇 %d 個選項:\n", e.min_cnt);
	}
	
	LIST_FOR_EACH(pNode, e.selections) {
		printf("%s\n", (char*)pNode->data);
	}
}

void display_selected(__attribute__((unused)) sGame *pGame, __attribute__((unused)) i32 viewer_id, bool *selected, i32 size) {
	printf("目前已經選取的選項: [");
	i32 cnt = 0;
	for(i32 i = 0; i < size; ++i) {
		if(selected[i]) {
			if(cnt != 0) printf(", ");  // [1, 2, 3, 4]: first no ", "
			printf("%d", i+1);
			++cnt;
		}
	}
	printf("]\n");
}

void display_end(__attribute__((unused)) sGame *pGame, eRole role) {
	char msg[4][32];
	strcpy(msg[Sheriff],  "Sheriff  IS THE WINNER");
	strcpy(msg[Outlaws],  "Outlaws  IS THE WINNER");
	strcpy(msg[Renegade], "Renegade IS THE WINNER");

	printf("--------------------------------------------------------------------------------\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||     '##:::::'##:'####:'##::: ##:'##::: ##:'########:'########::'####:      ||\n");
	printf("||     '##:'##: ##:. ##:: ###:: ##: ###:: ##: ##.....:: ##.... ##: ####:      ||\n");
	printf("||     '##: ##: ##:: ##:: ####: ##: ####: ##: ##::::::: ##:::: ##: ####:      ||\n");
	printf("||     '##: ##: ##:: ##:: ## ## ##: ## ## ##: ######::: ########::: ##::      ||\n");
	printf("||     '##: ##: ##:: ##:: ##. ####: ##. ####: ##...:::: ##.. ##::::..:::      ||\n");
	printf("||     '##: ##: ##:: ##:: ##:. ###: ##:. ###: ##::::::: ##::. ##::'####:      ||\n");
	printf("||     . ###. ###::'####: ##::. ##: ##::. ##: ########: ##:::. ##: ####:      ||\n");
	printf("||     :...::...:::....::..::::..::..::::..::........::..:::::..::....::      ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                      %s                                ||\n", msg[role]);
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                  ================================                          ||\n");
	printf("||                    Press any key to continue...                            ||\n");
	printf("||                  ================================                          ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("--------------------------------------------------------------------------------\n");
}

void start_style() {
	printf("--------------------------------------------------------------------------------\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||            '########:::::'###::::'##::: ##::'######:::'####:               ||\n");
	printf("||            '##.... ##:::'## ##::: ###:: ##:'##... ##:: ####:               ||\n");
	printf("||            '##:::: ##::'##:. ##:: ####: ##: ##:::..::: ####:               ||\n");
	printf("||            '########::'##:::. ##: ## ## ##: ##::'####:: ##::               ||\n");
	printf("||            '##.... ##: #########: ##. ####: ##::: ##:::..:::               ||\n");
	printf("||            '##:::: ##: ##.... ##: ##:. ###: ##::: ##::'####:               ||\n");
	printf("||            '########:: ##:::: ##: ##::. ##:. ######::: ####:               ||\n");
	printf("||            :........:::..:::::..::..::::..:::......:::....::               ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                                            ||\n");
	printf("||                                                          "RED"      .######.    "RST"||\n");
	printf("||                     player required ( 4 ~ 7 )            "RED"    .#####"BL"#####   "RST"||\n");
	printf("||                                                          "RED"   :#####"BL"######   "RST"||\n");
	printf("||                                                          "RED"   :######"BL"#####   "RST"||\n");
	printf("||                  ================================        "RED"   :##########    "RST"||\n");
	printf("||                    Press any key to continue...          "RED"    .########     "RST"||\n");
	printf("||                  ================================        "RED"     .##   ##     "RST"||\n");
	printf("||                                                          "RED"     .##   ##     "RST"||\n");
	printf("||                                                                            ||\n");
	printf("--------------------------------------------------------------------------------\n");
}

void color() {
	printf(RED"RED\n");
	printf(GRN"GRN\n");
	printf(YLW"YLW\n");
	printf(BL"BL\n");
	printf(MGT"MGT\n");
	printf(CYN"CYN\n");
	printf(GRY"GRY\n");
	printf(LRED"LRED\n");
	printf(LGRN"LGRN\n");
	printf(LYLW"LYLW\n");
	printf(LBL"LBL\n");
	printf(LMGT"LMGT\n");
	printf(LCYN"LCYN\n");
	printf(LWHT"LWHT\n");
}