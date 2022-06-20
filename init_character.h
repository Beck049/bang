#pragma once

#include "basic.h"
#include "cJSON.h"

typedef enum _eCharacter {
    WILLY_THE_KID = 0,
    BART_CASSIDY,
    BLACK_JACK,
    CALAMITY_JANET,
    EL_GRINGO,
    JESSE_JONES,
    JOURDONNAIS,
    KIT_CARLSON,
    LUCKY_DUKE,
    PAUL_REGRET,
    PEDRO_RAMIREZ,
    ROSE_DOOLAN,
    SID_KETCHUM,
    SLAB_THE_KILLER,
    SUZY_LAFAYETTE,
    VALTURE_SAM,
} _eCharacter;

typedef struct _sCharacters {
    int id;
    int hp;
    char name[16];
    char description[512];
} sCharacter;

extern int character_num;
extern sCharacter characters[16];
extern cJSON *json_characters;

void read_character( sCharacter *character, cJSON *obj );

void init_characters();