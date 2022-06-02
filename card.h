#pragma once

#include "basic.h"
#include "cJSON.h"

enum e_suits { SPADE, HEART, DIAMOND, CLUB };
typedef enum e_suits suits;

typedef struct _sCard {
    int  id ; // specific card id
    int type; // card type id

    suits suit;
    int  num;

    char name[16];
    char description[128];
}sCard;

int card_num;
sCard cards[80];

void read_card( sCard *card, cJSON *obj ) {

    cJSON *id = cJSON_GetObjectItem(obj, "id");
    cJSON *type = cJSON_GetObjectItem(obj, "type");
    cJSON *suit = cJSON_GetObjectItem(obj, "suit");
    cJSON *num  = cJSON_GetObjectItem(obj, "num");
    cJSON *name  = cJSON_GetObjectItemCaseSensitive(obj, "name");
    cJSON *description  = cJSON_GetObjectItemCaseSensitive(obj, "description");
    

    printf("%d\t", id->valueint);
    printf("%d\t", type->valueint);
    printf("%d\t", suit->valueint);
    printf("%d\t", num->valueint);
    printf("%s\t", name->valuestring);
    printf("%s\n", description->valuestring);

    card->id   = id  ->valueint;
    card->type = type->valueint;
    card->suit = suit->valueint;
    card->num  = num ->valueint;
    strcpy(card->name, name->valuestring);
    strcpy(card->description, description->valuestring);

    return;
}

void init_cards() {

    FILE* pFile = NULL;
    if( ( pFile = fopen( "card_data.json", "rb" ) ) == NULL ) {
        printf( "File could not be opened!\n" );
        return;
    }

    char cur_str[BUFSIZ] = {0};
    if( fread(cur_str, 1, sizeof(cur_str), pFile) <= 0 ) {
        printf( "Failed to read!\n" );
    }
    cJSON *arr = (cJSON*)cJSON_Parse(cur_str);
    card_num = cJSON_GetArraySize(arr);

    for( int i = 0; i < card_num; ++i) {
        cJSON *elem = NULL;
        elem = cJSON_GetArrayItem(arr, i);
        read_card( &cards[i], elem );
    }

    return;
}

