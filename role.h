#pragma once

#include "basic.h"

typedef enum _eRole {
    Sheriff = 0,
    Renegade,
    Outlaws,
    Deputy,
} eRole;


// usage: ROLE_DIVIDED[players][role]
static const i32 ROLE_DIVIDED[8][4] = {
    {0, 0, 0, 0},  // 0
    {0, 0, 0, 0},  // 1
    {0, 0, 0, 0},  // 2
    {0, 0, 0, 0},  // 3
    {1, 1, 2, 0},  // 4
    {1, 1, 2, 1},  // 5
    {1, 1, 3, 1},  // 6
    {1, 1, 3, 2},  // 7 players
};