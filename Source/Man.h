namespace game_framework
{
    class Man
    {
    public:
        Man();
        virtual ~Man();

        virtual void LoadBitMap() = 0;
        void OnMove(int seed);
        void OnShow(CGameMap* map);
        virtual void OnBeginState() = 0;													// 初始化狀態
        int GetX();																			// 取得男生X座標 (地圖座標)
        int GetY();																			// 取得男生Y座標 (地圖座標)
        int GetWidth();																		// 取得男生寬度
        int GetHeight();																	// 取得男生高度
        int GetId();																		// 取得男生ID
        double GetHP();																		// 取得男生血量
        int GetScore();																		// 取得該男生的分數
        bool HitMainGirl(CGameMap* map, MainGirl* girl);									// 是否與女主角接觸
        void SetIsAlive(bool status);														// 設定存亡
        void SetIsFocused(bool status);														// 設定是否被鎖定
        void SetIsAttackedBy(int who);														// 設定被誰攻擊
        bool IsAttackedBy(int who);															// 是否被此人攻擊
        bool IsAlive();																		// 是否存活
        bool IsAlreadyDead();																// 是否已經死透
        bool IsOver();																		// 是否已經與其他女生離開
        bool IsFocused();																	// 是否被鎖定
        void LoseHP(double value);															// 扣除血量
        void SetIsKilledBy(int who);														// 設定被誰殺死
        bool IsKilledBy(int who);															// 是否被此人殺死
        void SetIsFollowing(int who);														// 設定跟隨某人
        bool IsFollowing(int who);															// 是否跟隨某人
        //only affect when status is following
        void Follow(int x, int y, bool direction);											// 跟隨座標點
        void Report();																		// 回報分數動畫
        bool IsReporting();																	// 是否正在回報分數
        static int mainGirl;																// 女主角
        static int all;																		// 女主角+女生
        static int girl;																	// 女生
    protected:
        void SetDirection(bool direction);													// 設定移動方向
        void DrawClickingProgress(CGameMap* map);											// 畫出搶奪進度條
        int id;																				// ID
        int score;																			// 該男生的分數
        int x, y;																			// 男生位置
        int fx, fy;																			// 跟隨的終點位置
        bool fdirection;																	// 跟隨的最終移動方向
        bool is_positioned;																	// 是否跟隨到定點
        bool moving;																		// 是否正在移動
        bool direction;																		// false => 往左, true => 往右
        int velocity;																		// 移動速度
        double HP;																			// 血量
        int range[2];																		// 移動範圍
        int status;																			// 狀態
        int is_killed_by;																	// 被誰殺死
        bool is_focused;																	// 是否被鎖定
        static bool bitmapIsLoaded;															// bitmap素材是否已載入
        int distance;																		// 與其他女生離開的剩餘距離
        bool is_reporting;																	// 是否正在回報分數
        CAnimation man_left, man_right;
        CMovingBitmap man_left_stand, man_right_stand;
        CAnimation man_dead_left, man_dead_right;
        CAnimation man_following_left, man_following_right;
        CAnimation flash, flash_multi, weakening;
        CAnimation blood;
        CAnimation man_following_girl_left, man_following_girl_right;
        CAnimation scoreReport;
        static CAnimation clicking;															// 搶奪用量條動畫
        static CMovingBitmap clicking_bar;													// 搶奪用量條
        CPoint color_point[2];
    };

    class NormalMan : public Man
    {
    public:
        NormalMan(int x, int y, int start, int end, bool direction, int type);
        ~NormalMan();
        void LoadBitMap();
        void OnBeginState();																// 初始化狀態
    private:
        // 用於儲存原始狀態
        const int type;																		// 人物類型
        const int x;																		// 人物X座標 (地圖座標)
        const int y;																		// 人物Y座標 (地圖座標)
        const int start;																	// 移動的左邊範圍邊界 X軸
        const int end;																		// 移動的右邊範圍邊界 X軸
        const bool direction;																// 移動方向
    };

    class SpecialMan : public Man
    {
    public:
        SpecialMan(int x, int y, int start, int end, bool direction, int type);
        ~SpecialMan();
        void LoadBitMap();
        void OnBeginState();																// 初始化狀態
    private:
        // type == 1 => Blue hair
        // type == 2 => Brown hair
        // type == 3 => Gold hair
        // 用於儲存原始狀態
        const int type;																		// 人物類型
        const int x;																		// 人物X座標 (地圖座標)
        const int y;																		// 人物Y座標 (地圖座標)
        const int start;																	// 移動的左邊範圍邊界 X軸
        const int end;																		// 移動的右邊範圍邊界 X軸
        const bool direction;																// 移動方向
    };
}
