namespace game_framework
{
    class Heart
    {
    public:
        Heart(int pos, int size, int x, int y, int numberOfPeople);
        void OnMove();
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl); // 是否與女主角接觸
        int GetHP(); // 取得愛心量
    private:
        void LoadBitmap();
        int size; // 0=>normal 1=>big
        int HP;	// 愛心量
        int x, y; // 愛心位置
        int pos; // 上/下
        CAnimation heart;
    };
}