namespace game_framework
{
    class Girl
    {
    public:
        Girl(int x, int y, int start, int end, bool direction);
        virtual void LoadBitMap() = 0;
        bool IsNearBy(int x, int y);
        void SetMoving(bool status);
        void SetDirection(bool direction);
        void OnMove();
        void OnShow(CGameMap* map);
    protected:
        int x, y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        int range[2];
        CAnimation girl_left, girl_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
    };

    class NormalGirl : public Girl
    {
    public:
        NormalGirl(int x, int y, int start, int end, bool direction, int type);
        void LoadBitMap();
    private:
        const int type;
    };
}


