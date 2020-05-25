#include <string>
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
    private:
        void ParseCsv(std::string name, int array[][24]);
        CMovingBitmap firstFloor, secondFloor, thirdFloor, ceiling;
        int sx, sy;
    };
}


