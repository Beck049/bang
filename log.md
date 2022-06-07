#### log

2022/06/07
**game.h**
void play_game(i32 num_players)  
    開始遊戲
    宣告 pGame
    呼叫 game_init

void game_init(sGame *pGame)
    遊戲初始化
    設定遊玩人數
    洗牌,呼叫 draw_pile_init
    設定角色,呼叫 players_setup

void draw_pile_init(sGame *pGame)
    洗牌
    
void players_setup(sGame *pGame)
    設定角色