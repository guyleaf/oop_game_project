namespace game_framework
{

    class CGameStateInit : public CGameState
    {
    public:
        CGameStateInit(CGame* g, int* score, bool* isDead);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);

    protected:
        void OnMove();
        void OnShow();									// 顯示這個狀態的遊戲畫面
        int cursor_x1, cursor_y1, cursor_x2, cursor_y2, view, section, now;
        bool change;
        DWORD volume;
        int delay_counter;
        bool changeState;
        bool isLoaded;
        bool isPlaying;
    private:
        CAnimation intro[6];
        CMovingBitmap logo, about;								// csie的logo
        CMovingBitmap button1_1, button1_2, button2_1, button2_2;
        CMovingBitmap voice1, voice2, voice3, voice4, init_voice1, init_voice2, init_voice3, init_voice4;
        CMovingBitmap about_button, about_button_hover, intro_button, intro_button_hover;
		CMovingBitmap next, before, next_hover, before_hover, backInit, backInit_hover;
    };
}


