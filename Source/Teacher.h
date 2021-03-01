namespace game_framework
{
    class Teacher
    {
    public:
        Teacher(int level, int x, int y);
        void LoadBitmap();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl);			// 判斷是否與女主角接觸
        bool IsInLevel(int level);					// 老師是否在level層
    private:
        int x, y;									// 老師位置
        int level;									// 老師所在的樓層
        bool direction;								// 移動方向
        CAnimation left, right;
    };
}