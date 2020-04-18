namespace game_framework
{
    class CGameStateInit : public CGameState
    {
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);
		int GetCursorX1();
		int GetCursorY1();
		int GetCursorX2();
		int GetCursorY2();
		
    protected:
		void OnMove();
        void OnShow();									// 顯示這個狀態的遊戲畫面
		int cursor_x1, cursor_y1, cursor_x2, cursor_y2;
    private:
        CMovingBitmap logo;								// csie的logo
		CMovingBitmap button1_1, button1_2, button2_1, button2_2;
    };
}


