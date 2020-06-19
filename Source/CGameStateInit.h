namespace game_framework
{

    class CGameStateInit : public CGameState
    {
    public:
        CGameStateInit(CGame* g, int* score, bool* isDead);
        void OnInit();  																						// �C������Ȥιϧγ]�w
        void OnBeginState();																					// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 																		// �B�z��LUp���ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);															// �B�z�ƹ�������U��}���ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);															// �B�z�ƹ����зƹL���ʧ@
    protected:
        void OnMove();																							// �o�Ӫ��󪺰ʧ@���A
        void OnShow();																							// ��ܳo�Ӫ��A���C���e��
        int cursor_x1, cursor_y1, cursor_x2, cursor_y2, view, now;												// ���Ц�m�B�e�������B�����e���������ܼ�
        bool change;																							// �P�_�����R���P�_���ܼ�
        DWORD volume;																							// �x�s���֪��A���ܼ�
        int delay_counter;																						// ����ɶ�
        bool changeState;																						// �P�_�������A
        bool isLoaded;																							// �P�_LOADING���A
        bool isPlaying;																							// �P�_���ּ��񪬺A
    private:
        CAnimation intro[6];																					// �x�s�����Ϥ����}�C
        CMovingBitmap logo, about;																				// �I���ϡBAbout�e��
        CMovingBitmap button1_1, button1_2, button2_1, button2_2;												// �D�n���s:�}�l�P�����C��
        CMovingBitmap voice1, voice2, voice3, voice4, init_voice1, init_voice2, init_voice3, init_voice4;		// ���ֱ�����
        CMovingBitmap about_button, about_button_hover, intro_button, intro_button_hover;						// �i�J�C�������PAbout��icon���s
        CMovingBitmap next, before, next_hover, before_hover, backInit, backInit_hover;							// �W�@�ӻP�U�@�ӫ��s
    };
}


