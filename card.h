#pragma once
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

struct sCard {
    int  id ; // specific card id
    int type; // card type id

    enum suit { SPADE, HEART, DIAMOND, CLUB };
    int  num;

    char name[16];
    char description[128];
};

int card_num;
sCard cards[80];

void init( sCard *cards )
{
    FILE* pFile = NULL;
    if( ( pFile = fopen( "card_data.json", "rb" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return;
    }

    char cur_str[1024] = {0};
    while( !feof(pFile) )
    {
        memset(cur_str, 0, 1024);
        fgets(cur_str, 1024, pFile);
    }
}
