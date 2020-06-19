namespace game_framework
{

    class CGameStateInit : public CGameState
    {
    public:
        CGameStateInit(CGame* g, int* score, bool* isDead);
        void OnInit();  																						// 遊戲的初值及圖形設定
        void OnBeginState();																					// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 																		// 處理鍵盤Up的動作
        void OnLButtonUp(UINT nFlags, CPoint point);															// 處理滑鼠左鍵按下放開的動作
        void OnMouseMove(UINT nFlags, CPoint point);															// 處理滑鼠鼠標滑過的動作
    protected:
        void OnMove();																							// 這個物件的動作狀態
        void OnShow();																							// 顯示這個狀態的遊戲畫面
        int cursor_x1, cursor_y1, cursor_x2, cursor_y2, view, now;												// 鼠標位置、畫面切換、說明畫面切換的變數
        bool change;																							// 判斷音樂靜音與否的變數
        DWORD volume;																							// 儲存音樂狀態的變數
        int delay_counter;																						// 延遲時間
        bool changeState;																						// 判斷切換狀態
        bool isLoaded;																							// 判斷LOADING狀態
        bool isPlaying;																							// 判斷音樂播放狀態
    private:
        CAnimation intro[6];																					// 儲存說明圖片的陣列
        CMovingBitmap logo, about;																				// 背景圖、About畫面
        CMovingBitmap button1_1, button1_2, button2_1, button2_2;												// 主要按鈕:開始與結束遊戲
        CMovingBitmap voice1, voice2, voice3, voice4, init_voice1, init_voice2, init_voice3, init_voice4;		// 音樂控制鍵
        CMovingBitmap about_button, about_button_hover, intro_button, intro_button_hover;						// 進入遊戲說明與About的icon按鈕
        CMovingBitmap next, before, next_hover, before_hover, backInit, backInit_hover;							// 上一個與下一個按鈕
    };
}


