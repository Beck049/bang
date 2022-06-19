#include "init_card.h"

int card_num = 0;
sCard cards[80] = {};
cJSON *json_cards = NULL;

void read_card( sCard *card, cJSON *obj ) {

	cJSON *id = cJSON_GetObjectItem(obj, "id");
	cJSON *type = cJSON_GetObjectItem(obj, "type");
	cJSON *suit = cJSON_GetObjectItem(obj, "suit");
	cJSON *num  = cJSON_GetObjectItem(obj, "num");
	cJSON *name  = cJSON_GetObjectItemCaseSensitive(obj, "name");
	cJSON *description  = cJSON_GetObjectItemCaseSensitive(obj, "description");

	// printf("%d\t", id->valueint);
	// printf("%d\t", type->valueint);
	// printf("%d\t", suit->valueint);
	// printf("%d\t", num->valueint);
	// printf("%s\t", name->valuestring);
	// printf("%s\n", description->valuestring);

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
	if( ( pFile = fopen( "card_data_full.json", "rb" ) ) == NULL ) {
		printf( "File could not be opened!\n" );
		exit(1);
	}

	char buf[BUFSIZ<<2];
	if( fread(buf, 1, sizeof(buf), pFile) <= 0 ) {
		printf( "Failed to read!\n" );
		exit(1);
	}

	json_cards = (cJSON*)cJSON_Parse(buf);
	card_num = cJSON_GetArraySize(json_cards);

	for( int i = 0; i < card_num; ++i ) {
		cJSON *elem = cJSON_GetArrayItem(json_cards, i);
		read_card( &cards[i], elem );
	}
	
	fclose(pFile);
	return;
}
