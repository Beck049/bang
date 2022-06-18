#pragma once

#include "basic.h"
#include "list.h"
#include "role.h"

typedef struct _sPlayer {
	int id;
	int look_range;
	int be_looked_range;
	int attack_range;
	int hp;
	int character;
	eRole role;
	sList *cards;
	sList *desk;
} sPlayer;

sPlayer *new_player();

void free_player(sPlayer *pPlayer);