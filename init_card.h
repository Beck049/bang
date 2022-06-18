#pragma once

#include "basic.h"
#include "cJSON.h"

enum _eSuit { SPADE, HEART, DIAMOND, CLUB };
typedef enum _eSuit eSuit;

typedef struct _sCard {
	int  id ; // specific card id
	int  type; // card type id

	eSuit suit;
    int  num;
	char name[16];
	char description[512];
}sCard;

extern int card_num;
extern sCard cards[80];
extern cJSON *json_cards;

void read_card( sCard *card, cJSON *obj );

void init_cards();