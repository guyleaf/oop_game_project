namespace game_framework
{
    class Man
    {
    public:
        Man(int x, int y, int start, int end, bool direction);
        virtual void LoadBitMap() = 0;
        void SetMoving(bool status);
        void SetVelocity(int speed);
        void SetDirection(bool direction);
        void OnMove();
        void OnShow(CGameMap* map);
        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();
        int GetId();
        int GetHP();
        bool HitMainGirl(CGameMap* map, MainGirl* girl);
        void SetIsAlive(bool status);
        void SetIsFocused(bool status);
        bool IsAlive();
        bool IsFocused();
        void LoseHP(int value);

        //only affect when status is following
        void Follow(int x, int y, bool direction);
    protected:
        int id;
        int x, y;
        int fx, fy;
        bool fdirection;
        bool is_positioned;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        int HP;
        int range[2];
        int status;
        bool is_focused;
        bool is_attacked;
        CAnimation man_left, man_right;
        CMovingBitmap man_left_stand, man_right_stand;
        CAnimation man_dead_left, man_dead_right;
        CAnimation man_following_left, man_following_right;
        CAnimation flash, weakening;
        CAnimation blood;
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
