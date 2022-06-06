#pragma once

#include "basic.h"
#include "list.h"
#include "role.h"

typedef struct _sPlayer {
	int id;
	int attack_range;
	int hp;
	int character;
	enum eRole role;
	sList *cards;
	sList *desk;
} sPlayer;

sPlayer *new_player() {
	sPlayer *pPlayer = calloc(1, sizeof(sPlayer));
	return pPlayer;
}