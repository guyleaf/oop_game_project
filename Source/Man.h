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
        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();
        bool HitMainGirl(CGameMap* map, MainGirl* girl);
        void SetIsAlive(bool status);
        void SetIsFocused(bool status);
        bool IsAlive();
        bool IsFocused();
    protected:
        int x, y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        int score;
        int range[2];
        bool is_alive;
        bool is_focused;
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
