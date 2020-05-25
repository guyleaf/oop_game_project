namespace game_framework
{
    class UI
    {
    public:
        UI();
        void LoadVolume();
        void LoadBitmap();
        void OnMove();
        void OnShow(CGameMap* map);
        void OnMouseMove(CPoint point);
        void AddScore(int num);
        void Toggle();
        void Pause();
        void Resume();
        bool IsAudioButtonHoverd();
        int GetScore();
        void AddHeartPoints(int points);
        void SetHeartPoints(int points);
        int GetHeartPoints();
        void GotoHRState(int state);
        bool IsGameOver();
        void SetIsButtonVisible(bool status, bool direction);
        bool IsUpButtonHoverd();
        bool IsDownButtonHoverd();
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
        CMovingBitmap clock_background, clock;
        CPoint time_start, time_end;
        CPoint clock_center;
        CBitmap pointer;
        BITMAP pointer_size;
        XFORM xform;
        CMovingBitmap audio_button_on, audio_button_on_hovered, audio_button_off, audio_button_off_hovered;
        int cursor_x, cursor_y;
        bool is_muted;
        DWORD volume;
        CMovingBitmap up, up_hover;
        CMovingBitmap down, down_hover;
        bool rightButton;
        bool leftButton;
    };
}


