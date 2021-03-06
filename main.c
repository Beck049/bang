// #include "init_card.h"
// #include "init_character.h"
#include "event.h"
#include "select.h"
#include "draw.h"
#include "display.h"
#include "determine.h"
#include "bang.h"
#include "play_phase.h"

#include <unistd.h>
#include <getopt.h>

struct option long_options[] = 
{  
     { "people", 2 , NULL, 'p' },  
     { "help"  , 0 , NULL, 'h' },  
     {  0      , 0 , 0   , 0   },
};  

int main( int argc, char *argv[] ) {
	int32_t c = 0;
    int32_t index = 0;
    int32_t people = 4;
	int32_t flag_help = 0;
	int32_t flag_people = 0;

    while ( ( c = getopt_long( argc, argv, "p:h", long_options, &index ) ) != -1 ){
        switch( c ){
            case 'p':
				flag_people=1;
                people=atoi(optarg);
				if( people>7 || people<4 ){
					printf("不能 %d 個人遊玩\n",people);
					exit(1);
				}
                break;
            case 'h':
				flag_help=1;
                break;
           	default:
                printf( "option: unknown\n" ); 
                exit(1);
        }
    }
	
	if( flag_help==1 ){
		printf("…..*..lovelovelo…*\n");
		printf("…*..lovelovelove….*\n");
		printf("..*.lovelovelovelove…*…………….*….*\n");
		printf(".*..lovelovelovelovelo…*………*..lovel….*\n");
		printf("...*..lovelovelovelovelove…*….*…lovelovelo.*\n");
		printf("*.. lovelovelovelovelove…*….*…lovelovelo.*\n");
		printf(".*..lovelovelovelovelove…*..*…lovelovelo…*\n");
		printf("..*…lovelovelovelovelove..*…lovelovelo…*'########::'########:::'#######:::'######:::'########:::::'###::::'##::::'##:'##::::'##:'####:'##::: ##::'######:::\n");
		printf("…*….lovelovelolovelovelovelovelovelo…* ##.... ##: ##.... ##:'##.... ##:'##... ##:: ##.... ##:::'## ##::: ###::'###: ###::'###:. ##:: ###:: ##:'##... ##::\n");
		printf("…..*….lovelovelovelovelovelovelov…*    ##:::: ##: ##:::: ##: ##:::: ##: ##:::..::: ##:::: ##::'##:. ##:: ####'####: ####'####:: ##:: ####: ##: ##:::..:::\n");
		printf("……..*….lovelovelovelovelovelo…*        ########:: ########:: ##:::: ##: ##::'####: ########::'##:::. ##: ## ### ##: ## ### ##:: ##:: ## ## ##: ##::'####:\n");
		printf("………..*….lovelovelovelove…*             ##.....::: ##.. ##::: ##:::: ##: ##::: ##:: ##.. ##::: #########: ##. #: ##: ##. #: ##:: ##:: ##. ####: ##::: ##::\n");
		printf("……………*…lovelovelo….*                   ##:::::::: ##::. ##:: ##:::: ##: ##::: ##:: ##::. ##:: ##.... ##: ##:.:: ##: ##:.:: ##:: ##:: ##:. ###: ##::: ##::\n");
		printf("………………*..lovelo…*                      ##:::::::: ##:::. ##:. #######::. ######::: ##:::. ##: ##:::: ##: ##:::: ##: ##:::: ##:'####: ##::. ##:. ######:::\n");
		printf("…………………*…..*                          ..:::::::::..:::::..:::.......::::......::::..:::::..::..:::::..::..:::::..::..:::::..::....::..::::..:::......::::\n");
		printf("………………….*..*\n");
		
		printf("\t\t\t\t\toptions: \n");
		printf("\t\t\t\t\t[ -h ] : aka --help , 印出此表。\n");
		printf("\t\t\t\t\t[ -p ] : aka --people , 後方代的是遊玩的人數 , BANG的人數限制是4~7人.\n\n\n\n");
		if(flag_people){

		}else return 0;
	}

	start_style();
	play_game(people);

	return 0;
}
