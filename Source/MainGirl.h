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
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        CMovingBitmap girl;
    };
}


