#pragma once

#include "basic.h"
#include "cJSON.h"

typedef enum _eSuit {
	SPADE,
	HEART,
	DIAMOND,
	CLUB,
} eSuit;

typedef enum _eCardType {
	MISS,
	BANG,
	INDIANS,
	DUEL,
	SALOON,
	GENERAL_STORE,
	STAGECOACH,
	WELLS_FARGO,
	PANIC,
	GATLING,
	CAT_BALOU,
	BEER,
	REMINGTON,
	SCHOFIELD,
	WINCHESTER,
	CARABINE,
	VOLCANIC,
	SCOPE,
	MUSTANG,
	JAIL,
	BARREL,
	BOMB,
} eCardType;

typedef struct _sCard {
	int  id ; // specific card id
	eCardType type; // card type id

	eSuit suit;
    int  num;
	char name[64];
	char description[512];
}sCard;

extern int card_num;
extern sCard cards[80];
extern cJSON *json_cards;

void read_card( sCard *card, cJSON *obj );

void init_cards();