namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的結束狀態(Game Over)
    // 每個Member function的Implementation都要弄懂
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateOver : public CGameState
    {
    public:
        CGameStateOver(CGame* g, int* score, bool* isDead);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
        void OnMouseMove(UINT nFlags, CPoint point);
        void OnLButtonDown(UINT nFlags, CPoint point);
        void OnLButtonUp(UINT nFlags, CPoint point);
    private:
        bool loop;
        bool change;
        DWORD volume;
        int section;
        int cursor_x, cursor_y;
        bool playAgain;
        int counter;
        CAnimation starts[6], loops[6];
        CMovingBitmap voice_button_on, voice_button_off, voice_button_on_hover, voice_button_off_hover;
        CMovingBitmap restart, restart_hover;
        CMovingBitmap exit, exit_hover;
        CMovingBitmap num[10];
    };
}

