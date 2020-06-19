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
        void OnLButtonUp(UINT nFlags, CPoint point);
    private:
        bool loop;										// �i�Jloop�ʵe�`��
        bool change;									// �R�����sĲ�o
        DWORD volume;									// ���q����
        int section;									// ���@�ص���
        int cursor_x, cursor_y;							// �ƹ���m
        bool playAgain;									// �^��D���
        int counter;									// ����
        CAnimation starts[6], loops[6];					// �`�@��6�ӳ���
        CMovingBitmap voice_button_on, voice_button_off, voice_button_on_hover, voice_button_off_hover;
        CMovingBitmap restart, restart_hover;
        CMovingBitmap exit, exit_hover;
        CMovingBitmap num[10];
    };
}

