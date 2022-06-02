#pragma once

#include "basic.h"
#include "cJSON.h"

typedef struct _sCharacters {
    int id;
    char name[16];
    char description[128];
}sCharacter;

int character_num = 16;
sCharacter characters[16];

cJSON *json_characters;

void read_character( sCharacter *character, cJSON *obj ) {

    cJSON *id = cJSON_GetObjectItem(obj, "id");
    cJSON *name  = cJSON_GetObjectItemCaseSensitive(obj, "name");
	cJSON *description  = cJSON_GetObjectItemCaseSensitive(obj, "description");

    // printf("%d\t", id->valueint);
    // printf("%s\t", name->valuestring);
	// printf("%s\n", description->valuestring);

    character->id   = id  ->valueint;
    strcpy(character->name, name->valuestring);
	strcpy(character->description, description->valuestring);

	return;
}

void init_characters() {
    FILE* pFile = NULL;
	if( ( pFile = fopen( "character_data.json", "rb" ) ) == NULL ) {
		printf( "File could not be opened!\n" );
		exit(1);
	}

    char buf[BUFSIZ];
	if( fread(buf, 1, sizeof(buf), pFile) <= 0 ) {
		printf( "Failed to read!\n" );
		exit(1);
	}

    json_characters = (cJSON*)cJSON_Parse(buf);

	for( int i = 0; i < character_num; ++i ) {
		cJSON *elem = cJSON_GetArrayItem(json_characters, i);
		read_character( &characters[i], elem );
	}

    fclose(pFile);
    return;
}