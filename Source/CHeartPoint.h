namespace game_framework
{
    class CHeartPoint
    {
    public:
        CHeartPoint(int, int = 5);		// default 5 Hearts
        void Add(int x);			// 增加Point值
        int  GetPoint();			// 回傳Point值
        void LoadBitmap();			// 載入愛心量條之圖形
        void SetPoint(int);			// 設定Point值
        void SetTopLeft(int, int);	// 將動畫的左上角座標移至 (x,y)
        void OnMove();
        void OnShow(int counter);			// 將動畫貼到螢幕
        void GotoHRState(int state);
        static int normal;
        static int reinforcing;
        static int reinforced;
    private:
        const int NUMHEARTS;			// 共顯示NUMDIGITS個位數
        const int NUMPOINTSPERHEART;
        CMovingBitmap hearts[20];		// 儲存愛心量條之圖形(bitmap)
        CAnimation star, reinforced_bar;
        int state;
        int x, y;						// 顯示的座標
        int n;							// Point值
        bool isBmpLoaded;				// 是否已經載入圖形
    };
}

