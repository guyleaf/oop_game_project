namespace game_framework
{
    class Role
    {
    public:
        int GetX();																			// 取得人物X座標 (地圖座標)
        int GetY();																			// 取得人物Y座標 (地圖座標)
        int GetWidth();																		// 取得人物寬度
        int GetHeight();																	// 取得人物高度
    protected:
        int x, y;																			// 人物位置
        bool moving;																		// 是否正在移動
        bool direction;																		// false => 往左, true => 往右
        int velocity;																		// 移動速度
        int range[2];																		// 移動範圍
        int distance;																		// 離開時的剩餘距離
        int status;																			// 狀態
        CAnimation left, right;
        CMovingBitmap left_stand, right_stand;
    };
}


