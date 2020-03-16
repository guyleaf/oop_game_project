#include <string>
#define MAP_W 2315
namespace game_framework
{
    class CGameMap
    {
    public:
        CGameMap();
        void LoadBitMap();
        void OnShow(int level);
        void Addsx(int val);
        void Addsy(int val);
        int ScreenX(int val);
        int ScreenY(int val);
        bool IsEmpty(int x, int y);
    private:
        void ParseCsv(std::string name, int array[][24]);
        CMovingBitmap firstFloor, secondFloor, ceiling;
        int sx, sy;
        const int WIDTH;
    };
}


