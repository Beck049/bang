#pragma once

#include "basic.h"
#include "list.h"

// 把 當前角色linked list 裡面的牌拿出來放到陣列裡面，讓玩家選擇要棄掉哪些牌。

void discard( sGame *pGame ){
    i32 id=*(i32 *)pGame->cur_player->data;
    i32 size=pGame->players[ id ]->cards.size;
    sListNode table[ size ];
    sListNode pNode=pGame->player[id]->cards->end.next;

    for( i32 i=0 ; i<size ; ++i ){
        table[i]=pNode;
        pNode=pNode.next;
    }

    for( i32 i=0 ; i<size ; ++i){
        printf(" - %d : %s\n",i, cards[table[i].data].name );
        printt(" -      %s\n",cards[table[i].data].discription);
    }

    // 用 list_push_front 放進 discard_pile 拜託
}

void inquire( sListNode *table ){
    printf("==========================================\n")
    printf("Please enter the number of card to discard it: ");
}