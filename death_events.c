#include "death_events.h"

void death_event_default(sGame *pGame, sDeathEvent *e) {
    pGame->end_winner_role = getWinner( pGame );
}

int getWinner( sGame *pGame ){
    //預設：-1   警長/副警長贏的話：0   叛徒贏的話：1   歹徒贏的話：2
    int32_t id_list[ pGame->total_players ];
    int32_t role_list[4];
    int cnt=0;

    LIST_FOR_EACH( pNode , pGame->live_players ){
        id_list[cnt] = *(int32_t *)pNode->data;
        cnt++;
    }
    
    for(int32_t i=0 ; i<cnt ; ++i){
        int32_t role=pGame->players[ role_list[i] ].role;
        role_list[role]++;
    }
    
    /*  [role_list的順序] 警長：0 ; 副警長:1 ; 歹徒:2 ; 叛徒:3
        警長死了{
            叛徒獨活->叛徒贏
            其他->歹徒贏
        }
        警長沒死{
            除了警長副警長外的人都死了，警長副警長贏
        }
    */

    if( role_list[0]==0 ){
        if( role_list[3]==1 ){
            return 1;
        }else{
            return 2;
        }
    }else{
        if( role_list[2]==0 && role_list[3]==0 ){
            return 0;
        }
    }

    return -1;
}