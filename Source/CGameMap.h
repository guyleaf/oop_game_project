#include <string>
namespace game_framework
{
    class CGameMap
    {
    public:
        CGameMap();
        void LoadBitMap();
        void OnBeginState();												// 初始化狀態
        void OnMove();
        void OnShow();
        void Addsx(int val);												// 移動地圖X位置
        void Addsy(int val);												// 移動地圖Y位置
        int Height();														// 取得高度
        int Width();														// 取得寬度
        int ScreenX(int val);												// 轉換地圖X座標至視窗X座標
        int ScreenY(int val);												// 轉換地圖Y座標至視窗Y座標
        bool IsMapChanging();												// 是否地圖正在切換
        void SetLevel(int level);											// 設定樓層
        int GetLevel();														// 取得樓層
        bool IsInScreen(int start_x, int end_x);							// 是否在視窗範圍裡
        bool IsEmpty(int x, int y);											// 是否該座標為空
    private:
        CMovingBitmap firstFloor, secondFloor, thirdFloor, ceiling;
        int sx, sy;															// 地圖座標
        int level;															// 樓層
        int height, width;													// 高度, 寬度
        int left_edge, right_edge;											// 左邊界線, 右邊界線
        int counter;														// 地圖轉場延遲
        bool is_mapChanging;												// 是否地圖正在切換
    };
}


