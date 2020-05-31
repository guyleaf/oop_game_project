namespace game_framework
{
    class Teacher
    {
    public:
        Teacher(int x, int y);
        void LoadBitmap();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl);
    private:
        int x, y;
        bool direction;
        CAnimation left, right;
    };
}