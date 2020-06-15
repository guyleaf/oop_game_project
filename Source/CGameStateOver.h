namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // �o��class���C�����������A(Game Over)
    // �C��Member function��Implementation���n����
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateOver : public CGameState
    {
    public:
        CGameStateOver(CGame* g, int* score, bool* isDead);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        void OnMouseMove(UINT nFlags, CPoint point);
        void OnLButtonDown(UINT nFlags, CPoint point);
        void OnLButtonUp(UINT nFlags, CPoint point);
    private:
        bool loop;
        bool change;
        DWORD volume;
        int section;
        int cursor_x, cursor_y;
        bool playAgain;
        int counter;
        CAnimation starts[6], loops[6];
        CMovingBitmap voice_button_on, voice_button_off, voice_button_on_hover, voice_button_off_hover;
        CMovingBitmap restart, restart_hover;
        CMovingBitmap exit, exit_hover;
        CMovingBitmap num[10];
    };
}

