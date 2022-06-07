#### log

2022/06/07 \
**game.h** \
void play_game(i32 num_players)\
    開始遊戲\
    1. 宣告 pGame\
    2. 呼叫 game_init

void game_init(sGame *pGame)\
    遊戲初始化\
    1. 設定遊玩人數\
    2. 洗牌,呼叫 draw_pile_init\
    3. 設定角色,呼叫 players_setup

void draw_pile_init(sGame *pGame)\
    洗牌
    
void players_setup(sGame *pGame)\
    設定角色

void turn_phase(sGame *pGame)\
    檢查遊戲狀態，目前會印出遊戲階段。

i32 draw_card(sGame *pGame,i32 player_id, size_t cnt)\
    抽卡\
    1. 目前牌堆空了會回傳-1然後爛掉，之後得補把其他牌洗進去的部分。\
    2. 為了因應panic，感覺要把這個函數分成 1.抽牌堆 2.抽別人的牌堆。\

void draw(sList *pList)\
   將會回傳牌堆最上面那張牌的pointer