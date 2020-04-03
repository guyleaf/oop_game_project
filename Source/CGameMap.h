#define MAP_W 2894
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
        int ScreenX(int val);
        int ScreenY(int val);
        void SetLevel(int level);
        int GetLevel();
        bool IsEmpty(int x, int y);
    private:
        CMovingBitmap firstFloor, secondFloor, ceiling;
        int sx, sy;
        int level;
    };
}


