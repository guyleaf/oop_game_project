namespace game_framework
{
    class Heart
    {
    public:
        Heart(int size, int x, int y, int HP);
        void OnMove();
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl);
        int GetHP();
    private:
        void LoadBitmap();
        int size; // 0=>normal 1=>big
        int HP;
        int x, y;
        CAnimation heart;
    };
}


