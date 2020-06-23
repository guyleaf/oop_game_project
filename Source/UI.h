namespace game_framework
{
    class UI
    {
    public:
        UI();
        void LoadVolume();													// Ū���n�����A
        void LoadBitmap();
        void OnBeginState();												// ��l�ƪ��A
        void OnMove();
        void OnShow(CGameMap* map);
        void OnMouseMove(CPoint point);										// �B�z�ƹ������޿�
        void AddScore(int num);												// �[����
        void Toggle();														// �R�����s����
        void Pause();														// �Ȱ�
        void Resume();														// �}�l
        bool IsAudioButtonHoverd();											// �ˬd�ƹ��O�_�b���s�W
        int GetScore();														// ���o��e����
        void AddHeartPoints(int points);									// �ɥR�R��
        void SetHeartPoints(int points);									// �]�w�R�߼ƶq
        int GetHeartPoints();												// ���o�R��
        void GotoHRState(int state);										// �R�߰O���O�i�Jstate�Ҧ�
        bool IsGameOver();													// �O�_�C������
        void SetIsGameOver(bool status);									// �]�w�C������
        void SetIsButtonVisible(bool status, bool direction);				// �]�w�O�_��ܤW�U�ӫ��s
        bool IsUpButtonHoverd();											// �W�ӫ��s
        bool IsDownButtonHoverd();											// �U�ӫ��s
        bool IsWin();														// Ĺ�����A�U�����C��
    private:
        void DrawPie();														// �e�����I��
        void RotatePointer();												// ����������w
        int counter;														// �p�ɾ�
        const int MaxTime;													// �C���`�ɪ�
        int time_left;														// �Ѿl�ɶ�
        bool is_reinforced;													// �S��ɶ��Ҧ�
        bool is_win;														// �O�_Ĺ
        int state;															// ���A
        int clock_radius;													// �����b�|
        float angle;														// ����
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
        int cursor_x, cursor_y;												// �ƹ���m
        bool is_muted;														// �O�_�R��
        DWORD volume;														// ��l���q
        CMovingBitmap up, up_hover;
        CMovingBitmap down, down_hover;
        bool rightButton;													// �a�ϳ̥k��ӱ���s
        bool leftButton;													// �a�ϳ̥���ӱ���s
    };
}