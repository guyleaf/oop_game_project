#include <string>
namespace game_framework
{
    class CGameMap
    {
    public:
        CGameMap();
        void LoadBitMap();
        void OnShow();
        void Addsx(int val);
        void Addsy(int val);
        int Height();
        int Width();
        int ScreenX(int val);
        int ScreenY(int val);
        void SetLevel(int level);
        int GetLevel();
        bool IsInScreen(int start_x, int end_x);
    private:
        CMovingBitmap firstFloor, secondFloor, ceiling;
        int sx, sy;
        int level;
        int height, width;
    };
}


