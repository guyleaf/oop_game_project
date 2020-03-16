namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        void LoadBitMap();
        void SetMoving(CGameMap* map, CPoint point);
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
    private:
        int x, y;
        int H, W;
        bool moving; //�O�_���b����
        bool direction; //false => ����, true => ���k
        CMovingBitmap girl;
    };
}


