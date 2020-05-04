namespace game_framework
{
    class CHeartPoint
    {
    public:
        CHeartPoint(int = 5);		// default 5 Hearts
        void Add(int x);			// �W�[Point��
        int  GetPoint();			// �^��Point��
        void LoadBitmap();			// ���J�R�߶q�����ϧ�
        void SetPoint(int);			// �]�wPoint��
        void SetTopLeft(int, int);	// �N�ʵe�����W���y�в��� (x,y)
        void ShowBitmap();			// �N�ʵe�K��ù�
    private:
        const int NUMHEARTS;			// �@���NUMDIGITS�Ӧ��
        CMovingBitmap hearts[19];		// �x�s�R�߶q�����ϧ�(bitmap)
        int x, y;						// ��ܪ��y��
        int n;							// Point��
        bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
    };
}

