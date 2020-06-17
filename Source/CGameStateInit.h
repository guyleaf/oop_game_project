namespace game_framework
{

    class CGameStateInit : public CGameState
    {
    public:
        CGameStateInit(CGame* g, int* score, bool* isDead);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);

    protected:
        void OnMove();
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
        int cursor_x1, cursor_y1, cursor_x2, cursor_y2, view, section, now;
        bool change;
        DWORD volume;
        int delay_counter;
        bool changeState;
        bool isLoaded;
        bool isPlaying;
    private:
        CAnimation intro[6];
        CMovingBitmap logo, about;								// csie��logo
        CMovingBitmap button1_1, button1_2, button2_1, button2_2;
        CMovingBitmap voice1, voice2, voice3, voice4, init_voice1, init_voice2, init_voice3, init_voice4;
        CMovingBitmap about_button, about_button_hover, intro_button, intro_button_hover;
		CMovingBitmap next, before, next_hover, before_hover, backInit, backInit_hover;
    };
}


