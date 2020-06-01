namespace game_framework
{
    class CHeartPoint
    {
    public:
        CHeartPoint(int, int = 5);		// default 5 Hearts
        void Add(int x);			// �W�[Point��
        int  GetPoint();			// �^��Point��
        void LoadBitmap();			// ���J�R�߶q�����ϧ�
        void SetPoint(int);			// �]�wPoint��
        void SetTopLeft(int, int);	// �N�ʵe�����W���y�в��� (x,y)
        void OnMove();
        void OnShow(int counter);			// �N�ʵe�K��ù�
        void GotoHRState(int state);
        static int normal;
        static int reinforcing;
        static int reinforced;
    private:
        const int NUMHEARTS;			// �@���NUMDIGITS�Ӧ��
        const int NUMPOINTSPERHEART;
        CMovingBitmap hearts[20];		// �x�s�R�߶q�����ϧ�(bitmap)
        CAnimation star, reinforced_bar;
        int state;
        int x, y;						// ��ܪ��y��
        int n;							// Point��
        bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
    };
}

