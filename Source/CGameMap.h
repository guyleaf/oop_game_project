#include <string>
namespace game_framework
{
    class CGameMap
    {
    public:
        CGameMap();
        void LoadBitMap();
        void OnMove();
        void OnShow();
        void Addsx(int val);
        void Addsy(int val);
        int Height();
        int Width();
        int ScreenX(int val);
        int ScreenY(int val);
        bool IsMapChanging();
        void SetLevel(int level);
        int GetLevel();
        bool IsInScreen(int start_x, int end_x);
        bool IsEmpty(int x, int y);
    private:
        CMovingBitmap firstFloor, secondFloor, ceiling;
        int sx, sy;
        int level;
        int height, width;
        int left_edge, right_edge;
        int counter;
        bool is_mapChanging;
    };
}


