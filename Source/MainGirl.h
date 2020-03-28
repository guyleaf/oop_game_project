namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        void LoadBitMap();
        void OnMouseMove(CGameMap* map, CPoint point);
        void SetMoving(CGameMap* map, CPoint point);
        void SetVelocity(CGameMap* map, CPoint point);
        int GetCursorX();
        int GetCursorY();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
    private:
        int x, y;
        int cursor_x, cursor_y;
        bool moving; //�O�_���b����
        bool direction; //false => ����, true => ���k
        int velocity;
        CAnimation girl_left, girl_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
    };
}


