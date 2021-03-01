namespace game_framework
{
    class Teacher
    {
    public:
        Teacher(int level, int x, int y);
        void LoadBitmap();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl);			// �P�_�O�_�P�k�D����Ĳ
        bool IsInLevel(int level);					// �Ѯv�O�_�blevel�h
    private:
        int x, y;									// �Ѯv��m
        int level;									// �Ѯv�Ҧb���Ӽh
        bool direction;								// ���ʤ�V
        CAnimation left, right;
    };
}