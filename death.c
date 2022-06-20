#include "death.h"

void death_event_default(sGame *pGame, sDeathEvent *e) {
    e->death_res = true;
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

    if( role_cnt[Sheriff]==0 ) {
        if(role_cnt[Renegade]==1 && pGame->live_players->size == 1) {
            return Renegade;
        }else{
            return Outlaws;
        }
    }else{
        if( role_cnt[Outlaws]==0 && role_cnt[Renegade]==0 ) {
            return Sheriff;
        }
    }

    return -1;
}

void death_event_vulture_sam(sGame *pGame, sDeathEvent *e) {

    // find sam
    i32 sam_player_id = -1;
    LIST_FOR_EACH(pNode, pGame->live_players) {
        if( *(i32*)pNode->data == 15 ) {
            sam_player_id = *(i32*)pNode->data;
        }
    }

    if( sam_player_id == -1 ) {
        death_event_default(pGame, e);
        return;
    }
    
    e->death_res = true;
    pGame->end_winner_role = getWinner( pGame );
    sPlayer *pDead = &pGame->players[e->dead_id];
    sPlayer *pKiller = &pGame->players[e->killer_id];
    sList *death_desk = pDead->desk;
    sList *death_hand = pDead->cards;
    sList *sam_hand= pGame->players[sam_player_id].cards;
    
    while(death_desk->size != 0) {
        i32 card_id = take_card(pGame, death_desk, 0);
        give_card(pGame, sam_hand, card_id, false);
    }
    while(death_hand->size != 0) {
        i32 card_id = take_card(pGame, death_hand, 0);
        give_card(pGame, sam_hand, card_id, false);
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