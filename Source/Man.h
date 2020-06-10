namespace game_framework
{
    class Man
    {
    public:
        Man(int x, int y, int start, int end, bool direction);
        virtual ~Man();
        virtual void LoadBitMap() = 0;
        void SetMoving(bool status);
        void SetVelocity(int speed);
        void SetDirection(bool direction);
        void OnMove(int seed);
        void OnShow(CGameMap* map);
        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();
        int GetId();
        double GetHP();
        int GetScore();
        bool HitMainGirl(CGameMap* map, MainGirl* girl);
        void SetIsAlive(bool status);
        void SetIsFocused(bool status);
        void SetIsAttackedBy(int who);
        bool IsAttackedBy(int who);
        bool IsAlive();
        bool IsAlreadyDead();
        bool IsOver();
        bool IsFocused();
        void LoseHP(double value);
        void SetIsKilledBy(int who);

        //only affect when status is following
        void Follow(int x, int y, bool direction);
        void Report();
        bool IsReporting();
        static int mainGirl;
        static int all;
        static int girl;
    protected:
        void DrawBeam(CGameMap* map);
        int id;
        int score;
        int x, y;
        int fx, fy;
        bool fdirection;
        bool is_positioned;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        double HP;
        int range[2];
        int status;
        int is_killed_by;
        bool is_focused;
        static bool bitmapIsLoaded;
        int distance;
        bool is_reporting;
        //bool is_attacked;
        CAnimation man_left, man_right;
        CMovingBitmap man_left_stand, man_right_stand;
        CAnimation man_dead_left, man_dead_right;
        CAnimation man_following_left, man_following_right;
        CAnimation flash, flash_multi, weakening;
        CAnimation blood;
        CAnimation man_following_girl_left, man_following_girl_right;
        CAnimation scoreReport;
        static CAnimation clicking;
        static CMovingBitmap clicking_bar;
        CPoint color_point[2];
    };

    class NormalMan : public Man
    {
    public:
        NormalMan(int x, int y, int start, int end, bool direction, int type);
        ~NormalMan();
        void LoadBitMap();
    private:
        const int type;
    };

    class SpecialMan : public Man
    {
    public:
        SpecialMan(int x, int y, int start, int end, bool direction, int type);
        ~SpecialMan();
        void LoadBitMap();
    private:
        // type == 1 => Blue hair
        // type == 2 => Brown hair
        // type == 3 => Gold hair
        const int type;
    };
}
