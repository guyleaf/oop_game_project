namespace game_framework
{
    class Girl
    {
    public:
        Girl(int x, int y, int start, int end, bool direction);
        void LoadBitmap();
        bool IsNearBy(int x, int y);
        void SetMoving(bool status);
        void SetDirection(bool direction);
        void OnMove();
        void OnShow(CGameMap* map);
    private:
        int x, y;
        bool moving; //�O�_���b����
        bool direction; //false => ����, true => ���k
        int velocity;
        int range[2];
        CAnimation girl_left, girl_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
    };
}


