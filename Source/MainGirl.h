namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        void LoadBitMap();
        void SetMoving(CGameMap* map, CPoint point);
        void SetVelocity(CGameMap* map, CPoint point);
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
    private:
        int x, y;
        const int height, width;
        bool moving; //�O�_���b����
        bool direction; //false => ����, true => ���k
        int velocity;
        CMovingBitmap girl;
    };
}


