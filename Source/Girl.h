namespace game_framework
{
    class Girl
    {
    public:
        Girl(int x, int y, int start, int end, bool direction);
        virtual void LoadBitMap() = 0;
        void SetMoving(bool status);
        void SetDirection(bool direction);
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        void SetIsShocking(bool status);
        bool IsShocking();
        void LockPerson(Man* man, CGameMap* map);
        void Attack(Man* man, CGameMap* map);
        bool IsLocked();
        void Win();
        void Lose();
        bool IsWinner();
        bool IsAlreadyDead();
        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();
        void AddSlave(Man* man);
    protected:
        void DrawBeam(CGameMap* map);
        int x, y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        int range[2];
        bool is_shocking;

        int status;
        CPoint beam_pos[4];
        CAnimation girl_left, girl_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
        CAnimation exclamation;
        CAnimation surprising_left, surprising_right;
        CMovingBitmap shooting_left, shooting_right;
        CMovingBitmap notice_left, notice_right;
        CAnimation flying_left, flying_right;
        static vector<Man*> slaves;
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


