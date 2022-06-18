#include "death_events.h"

void death_event_default(sGame *pGame, sDeathEvent *e) {
    e->death_res = true;
    list_erase(pGame->live_players, get_player(pGame, e->dead_id));
    pGame->end_winner_role = getWinner( pGame );
    sPlayer *pDead = &pGame->players[e->dead_id];
    sPlayer *pKiller = &pGame->players[e->killer_id];
    sList *death_desk = pDead->desk;
    sList *death_hand = pDead->cards;

    // 死掉的這個人的牌都得被丟到棄牌堆。
    while(death_desk->size != 0) {
        i32 card_id = take_card(pGame, death_desk, 0);
        give_card(pGame, pGame->discard_pile, card_id, false);
    }
    while(death_hand->size != 0) {
        i32 card_id = take_card(pGame, death_hand, 0);
        give_card(pGame, pGame->discard_pile, card_id, false);
    }

    // 如果警長殺死副警長，警長的手牌跟面前的紙牌都得丟到棄牌堆
    if(pDead->role==Deputy && pKiller->role==Sheriff ){
        sList *sheriff_desk = pKiller->desk;
        sList *sheriff_hand = pKiller->cards;

        while(sheriff_desk->size != 0) {
            i32 card_id = take_card(pGame, sheriff_desk, 0);
            give_card(pGame, pGame->discard_pile, card_id, false);
        }
        while(sheriff_hand->size != 0) {
            i32 card_id = take_card(pGame, sheriff_hand, 0);
            give_card(pGame, pGame->discard_pile, card_id, false);
        }
    }
    // 如果任何玩家消滅歹徒，可以抽三張牌作為獎勵。
    if(pDead->role==Outlaws ){
        for(i32 i = 0; i < 3; ++i) {
            i32 card_id = take_card(pGame, pGame->draw_pile, 0);
            give_card(pGame, pKiller->cards, card_id, true);
        }
    }
}

eRole getWinner( sGame *pGame ){
    //預設：-1   警長/副警長贏的話：0   叛徒贏的話：1   歹徒贏的話：2
    i32 role_cnt[ROLE_SIZ];

    LIST_FOR_EACH( pNode , pGame->live_players ){
        i32 player_id = *(int32_t *)pNode->data;
        eRole role = pGame->players[player_id].role;
        ++role_cnt[role];
    }
    
    /*  [role_cnt的順序] 警長：0 ; 副警長:1 ; 歹徒:2 ; 叛徒:3
        警長死了{
            叛徒獨活->叛徒贏
            其他->歹徒贏
        }
        警長沒死{
            除了警長副警長外的人都死了，警長副警長贏
        }
    */

    if( role_cnt[Sheriff]==0 ){
        if(role_cnt[Renegade]==1 && pGame->live_players->size == 1){
            return Renegade;
        }else{
            return Outlaws;
        }
    }else{
        if( role_cnt[Outlaws]==0 && role_cnt[Renegade]==0 ){
            return Sheriff;
        }
    }

    return -1;
}

void death_event_Vulture_Sam(sGame *pGame, sDeathEvent *e) {
    i32 hand_card_id_list[40]={0};
    i32 desk_card_id_list[40]={0};
    i32 hand_card_cnt=0;
    i32 desk_card_cnt=0;
    e->death_res = true;
    pGame->end_winner_role = getWinner( pGame );
    sList * death_desk = pGame->players[ e->dead_id ].desk;
    sList * death_hand = pGame->players[ e->dead_id ].cards;

    // 找山姆這個人的id
    i32 id_sam=0;
    for( i32 i=0 ; i < pGame->total_players ; ++i ){
        if( pGame->players[ i ].character==15 /*sam的id*/ ) id_sam=i;
    }

    // 死掉的這個人的牌都得被丟到山姆的手牌。
    LIST_FOR_EACH(pNode, death_desk ){
        desk_card_id_list[ desk_card_cnt ] = *(int32_t *)pNode->data;
        desk_card_cnt++;
    }
    LIST_FOR_EACH(pNode, death_hand ){
        hand_card_id_list[ hand_card_cnt ] = *(int32_t *)pNode->data;
        hand_card_cnt++;
    }
    
    for(i32 i=0 ; i<desk_card_cnt ; ++i ){
        give_card( pGame , pGame->players[ id_sam ].cards , desk_card_id_list[i] , true );
    }
    for(i32 i=0 ; i<hand_card_cnt ; ++i){
        give_card( pGame , pGame->players[ id_sam ].cards , hand_card_id_list[i] , true );
    }
    // 如果警長殺死副警長，警長的手牌跟面前的紙牌都得丟到棄牌堆。
    if( pGame->players[ e->dead_id ].role==Deputy && pGame->players[ e->killer_id ].role==Sheriff ){
        hand_card_cnt=0;
        desk_card_cnt=0;
        for(i32 i=0 ; i<40 ; ++i){
            hand_card_id_list[i]=0;
            desk_card_id_list[i]=0;
        }
        sList * sheriff_desk = pGame->players[ e->killer_id ].desk;
        sList * sheriff_hand = pGame->players[ e->killer_id ].cards;

        LIST_FOR_EACH( pNode, sheriff_desk ){
            desk_card_id_list[ desk_card_cnt ] = *(int32_t *)pNode->data;
            desk_card_cnt++;
        }
        LIST_FOR_EACH(pNode, sheriff_hand ){
            hand_card_id_list[ hand_card_cnt ] = *(int32_t *)pNode->data;
            hand_card_cnt++;
        }
        for(i32 i=0 ; i<desk_card_cnt ; ++i ){
            give_card( pGame , pGame->discard_pile , desk_card_id_list[i] , true );
        }
        for(i32 i=0 ; i<hand_card_cnt ; ++i){
            give_card( pGame , pGame->discard_pile , hand_card_id_list[i] , true );
        }

        free_list( sheriff_desk );
        free_list( sheriff_hand );
    }
    // 如果任何玩家消滅歹徒，可以抽三張牌作為獎勵。
    if( pGame->players[ e->dead_id ].role==Outlaws ){
        give_card( pGame , pGame->players[ e->killer_id ].cards , *(i32*)LIST_FRONT(pGame->draw_pile) , true );
        give_card( pGame , pGame->players[ e->killer_id ].cards , *(i32*)LIST_FRONT(pGame->draw_pile) , true );
        give_card( pGame , pGame->players[ e->killer_id ].cards , *(i32*)LIST_FRONT(pGame->draw_pile) , true );
    }

    free_list( death_desk );
    free_list( death_hand );
    
}