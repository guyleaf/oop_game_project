namespace game_framework
{
    class UI
    {
    public:
        UI();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void AddScore(int num);
        int GetScore();
        void SetHeartPoints(int points);
        void SetIsReinforced(bool status);
        bool IsReinforced();
        bool IsGameOver();
    private:
        void DrawPie();
        void RotatePointer();
        int counter;
        const int MaxTime;
        int time_left;
        bool is_reinforced;
        int state;
        int clock_radius;
        float angle;
        CHeartPoint heart;
        CMovingBitmap heartPointBoard;
        CMovingBitmap scoreBoard;
        CInteger score;
        vector<CAnimation> hearts;
        CMovingBitmap clock_background, clock;
        CPoint time_start, time_end;
        CPoint clock_center;
        CBitmap pointer;
        BITMAP pointer_size;
        XFORM xform;
        DWORD originalValue;
    };
}


