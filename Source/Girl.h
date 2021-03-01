namespace game_framework
{
    class Girl : public Role
    {
    public:
        Girl();
        virtual ~Girl();
        virtual void LoadBitMap() = 0;
        virtual void OnBeginState() = 0;													// 初始化狀態
        void SetDirection(bool direction);													// 設定移動方向
        void OnMove(CGameMap* map, int seed);												// 處理動作更新邏輯
        void OnShow(CGameMap* map);															// 處理顯示邏輯
        void SetIsShocking(bool status);													// 設定是否注意到女主角正在魅惑男生
        bool IsShocking();																	// 是否注意到女主角正在魅惑男生
        void LockPerson(Man* man, CGameMap* map);											// 鎖定男生
        void Attack(Man* man, CGameMap* map);												// 攻擊男生
        bool IsLocked();																	// 是否鎖定
        void Win();																			// 贏
        void Lose();																		// 輸
        bool IsAlive();																		// 是否存活
        bool IsAlreadyDead();																// 是否已經死透
        virtual bool IsSpecialGirl() = 0;													// 是否為金髮特殊女生
    protected:
        void DrawBeam(CGameMap* map);														// 畫出雷射光束
        bool is_shocking;																	// 是否注意到女主角正在魅惑男生
        int damage;																			// 攻擊力
        CPoint beam_pos[4];																	// 雷射光束位置
        CAnimation exclamation;
        CAnimation surprising_left, surprising_right;
        CMovingBitmap shooting_left, shooting_right;
        CMovingBitmap notice_left, notice_right;
        CAnimation flying_left, flying_right;
        CAnimation leaving_left, leaving_right;
        CAnimation fun;
    };

    class NormalGirl : public Girl
    {
    public:
        NormalGirl(int x, int y, int start, int end, bool direction, int type);
        ~NormalGirl();
        void LoadBitMap();
        void OnBeginState();																// 初始化狀態
        bool IsSpecialGirl();																// 是否為金髮特殊女生
    private:
        // 用於儲存原始狀態
        const int type;																		// 人物類型
        const int x;																		// 人物X座標 (地圖座標)
        const int y;																		// 人物Y座標 (地圖座標)
        const int start;																	// 移動的左邊範圍邊界 X軸
        const int end;																		// 移動的右邊範圍邊界 X軸
        const bool direction;																// 移動方向
    };

    class SpecialGirl : public Girl
    {
    public:
        SpecialGirl(int x, int y, int start, int end, bool direction, int type);
        ~SpecialGirl();
        void LoadBitMap();
        void OnBeginState();
        bool IsSpecialGirl();
    private:
        const int type;																		// 人物類型
        const int x;																		// 人物X座標 (地圖座標)
        const int y;																		// 人物Y座標 (地圖座標)
        const int start;																	// 移動的左邊範圍邊界 X軸
        const int end;																		// 移動的右邊範圍邊界 X軸
        const bool direction;																// 移動方向
    };
}