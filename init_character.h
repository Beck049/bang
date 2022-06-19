#pragma once

#include "basic.h"
#include "cJSON.h"

typedef struct _sCharacters {
    int id;
    int hp;
    char name[16];
    char description[512];
}sCharacter;

extern int character_num;
extern sCharacter characters[16];
extern cJSON *json_characters;

void read_character( sCharacter *character, cJSON *obj );

void init_characters();