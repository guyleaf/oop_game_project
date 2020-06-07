namespace game_framework
{
    class Teacher
    {
    public:
        Teacher(int level, int x, int y);
        void LoadBitmap();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl);
        bool IsInLevel(int level);
    private:
        int x, y;
        int level;
        bool direction;
        CAnimation left, right;
    };
}