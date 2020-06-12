namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // �o��class���C�����������A(Game Over)
    // �C��Member function��Implementation���n����
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateOver : public CGameState
    {
    public:
        CGameStateOver(CGame* g, int* score);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
    };
}

