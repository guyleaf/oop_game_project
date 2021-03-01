namespace game_framework
{
    class UI
    {
    public:
        UI();
        void LoadVolume();													// 讀取聲音狀態
        void LoadBitmap();
        void OnBeginState();												// 初始化狀態
        void OnMove();
        void OnShow(CGameMap* map);
        void OnMouseMove(CPoint point);										// 處理滑鼠移動邏輯
        void AddScore(int num);												// 加分數
        void Toggle();														// 靜音按鈕切換
        void Pause();														// 暫停
        void Resume();														// 開始
        bool IsAudioButtonHoverd();											// 檢查滑鼠是否在按鈕上
        int GetScore();														// 取得當前分數
        void AddHeartPoints(int points);									// 補充愛心
        void SetHeartPoints(int points);									// 設定愛心數量
        int GetHeartPoints();												// 取得愛心
        void GotoHRState(int state);										// 愛心記分板進入state模式
        bool IsGameOver();													// 是否遊戲結束
        void SetIsGameOver(bool status);									// 設定遊戲結束
        void SetIsButtonVisible(bool status, bool direction);				// 設定是否顯示上下樓按鈕
        bool IsUpButtonHoverd();											// 上樓按鈕
        bool IsDownButtonHoverd();											// 下樓按鈕
        bool IsWin();														// 贏的狀態下結束遊戲
    private:
        void DrawPie();														// 畫時鐘背景
        void RotatePointer();												// 旋轉時鐘指針
        int counter;														// 計時器
        const int MaxTime;													// 遊戲總時長
        int time_left;														// 剩餘時間
        bool is_reinforced;													// 特殊時間模式
        bool is_win;														// 是否贏
        int state;															// 狀態
        int clock_radius;													// 時鐘半徑
        float angle;														// 角度
        CHeartPoint heart;
        CMovingBitmap heartPointBoard;
        CMovingBitmap scoreBoard;
        CInteger score;
        CMovingBitmap clock_background, clock;
        CPoint time_start, time_end;
        CPoint clock_center;
        CBitmap pointer;
        BITMAP pointer_size;
        XFORM xform;
        CMovingBitmap audio_button_on, audio_button_on_hovered, audio_button_off, audio_button_off_hovered;
        int cursor_x, cursor_y;												// 滑鼠位置
        bool is_muted;														// 是否靜音
        DWORD volume;														// 原始音量
        CMovingBitmap up, up_hover;
        CMovingBitmap down, down_hover;
        bool rightButton;													// 地圖最右邊樓梯按鈕
        bool leftButton;													// 地圖最左邊樓梯按鈕
    };
}