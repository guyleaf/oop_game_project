namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的結束狀態(Game Over)
    // 每個Member function的Implementation都要弄懂
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateOver : public CGameState
    {
    public:
        CGameStateOver(CGame* g, int* score);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
    };
}

