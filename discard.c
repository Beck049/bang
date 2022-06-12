#include "discard.h"

void discard(sGame *pGame ){
    for(i32 i=0 ; i< (pGame->total_players) ; ++i){
        i32 size=(i32)(pGame->players[i].cards->size);
        i32 cur_hp=(int)(pGame->players[i].hp);
        sSelectEvent card_to_discard;
        sListNode *table[ size ];
        sListNode pNode;

        if(size>cur_hp){
            sListNode *pNode=pGame->players[i].cards->end->next;

            for( i32 i=0 ; i<size ; ++i ){
                //所有手牌都可選擇要不要棄，所以把所有的牌都寫進table裡面。
                table[i] = pNode;
                pNode = pNode->next;
            }
            
            //如果手牌比當前hp多，就要棄掉多餘的牌。
            card_to_discard = select_event_with_arr(pGame,i,size-cur_hp,size-cur_hp,table,size,32);
        }
        LIST_FOR_EACH(pNode,card_to_discard.select_res){
            take_card_by_id( pGame,pGame->players[i].cards, *(i32 *) ( table[ *(int*) (pNode->data) ]->data ) );
            give_card( pGame , pGame->discard_pile , *(i32 *) ( table[ *(int*) (pNode->data) ]->data ) , false );
            //list_push_front( pGame->discard_pile,table[ *(int*)(pNode->data) ] );
        }
    }
    // 用 list_push_front 放進 discard_pile 拜託
    // 好
}