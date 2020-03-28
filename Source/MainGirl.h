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
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        CAnimation girl_left, girl_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
    };
}


