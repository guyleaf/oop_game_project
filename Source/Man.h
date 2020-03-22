namespace game_framework
{
    class Man
    {
    public:
        Man(int x, int y, int start, int end, bool direction);
        virtual void LoadBitMap() = 0;
        void SetMoving(bool status);
        void SetDirection(bool direction);
        void OnMove();
        void OnShow(CGameMap* map);
    protected:
        int x, y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        int score;
        int range[2];
        CAnimation man_left, man_right;
        CMovingBitmap man_left_stand, man_right_stand;
    };

    class NormalMan : public Man
    {
    public:
        NormalMan(int x, int y, int start, int end, bool direction, int type);
        void LoadBitMap();
    private:
        const int type;
    };
}
