#pragma once

#include "basic.h"

#define ROLE_SIZ 4
#define MIN_PLAYERS 4
#define MAX_PLAYERS 7

typedef enum _eRole {
    Sheriff = 0,
    Deputy,
    Outlaws,
    Renegade,
} eRole;


// usage: ROLE_DIVIDED[players][role]
static const i32 ROLE_DIVIDED[MAX_PLAYERS+1][ROLE_SIZ] = {
    {0, 0, 0, 0},  // 0
    {0, 0, 0, 0},  // 1
    {0, 0, 0, 0},  // 2
    {0, 0, 0, 0},  // 3
    {1, 0, 2, 1},  // 4
    {1, 1, 2, 1},  // 5
    {1, 1, 3, 1},  // 6
    {1, 2, 3, 1},  // 7 players
};

__attribute__((unused)) static const char *ROLE_STR[ROLE_SIZ] = {
    "Sheriff",
    "Deputy",
    "Outlaws",
    "Renegade"
};