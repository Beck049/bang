#pragma once

#include "basic.h"
#include "list.h"

typedef struct _sPlayer {
	int id;
	int attack_range;
	int hp;
	int character;
	int role;
	sList *cards;
	sList *desk;
} sPlayer;

sPlayer *new_player() {
	sPlayer *pPlayer = calloc(1, sizeof(sPlayer));
	return pPlayer;
}