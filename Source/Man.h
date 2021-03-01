namespace game_framework
{
    class Man : public Role
    {
    public:
        Man();
        virtual ~Man();

        virtual void LoadBitMap() = 0;
        void OnMove(int seed);																// �B�z�ʧ@��s�޿�
        void OnShow(CGameMap* map);															// �B�z����޿�
        virtual void OnBeginState() = 0;													// ��l�ƪ��A
        int GetId();																		// ���o�k��ID
        double GetHP();																		// ���o�k�ͦ�q
        int GetScore();																		// ���o�Өk�ͪ�����
        bool HitMainGirl(CGameMap* map, MainGirl* girl);									// �O�_�P�k�D����Ĳ
        void SetIsAlive(bool status);														// �]�w�s�`
        void SetIsFocused(bool status);														// �]�w�O�_�Q��w
        void SetIsAttackedBy(int who);														// �]�w�Q�֧���
        bool IsAttackedBy(int who);															// �O�_�Q���H����
        bool IsAlive();																		// �O�_�s��
        bool IsAlreadyDead();																// �O�_�w�g���z
        bool IsOver();																		// �O�_�w�g�P��L�k�����}
        bool IsFocused();																	// �O�_�Q��w
        void LoseHP(double value);															// ������q
        void SetIsKilledBy(int who);														// �]�w�Q�ֱ���
        bool IsKilledBy(int who);															// �O�_�Q���H����
        void SetIsFollowing(int who);														// �]�w���H�Y�H
        bool IsFollowing(int who);															// �O�_���H�Y�H
        //only affect when status is following
        void Follow(int x, int y, bool direction);											// ���H�y���I
        void Report();																		// �^�����ưʵe
        bool IsReporting();																	// �O�_���b�^������
        static int mainGirl;																// �k�D��
        static int all;																		// �k�D��+�k��
        static int girl;																	// �k��
    protected:
        void SetDirection(bool direction);													// �]�w���ʤ�V
        void DrawClickingProgress(CGameMap* map);											// �e�X�m�ܶi�ױ�
        int id;																				// ID
        int score;																			// �Өk�ͪ�����
        int fx, fy;																			// ���H�����I��m
        bool fdirection;																	// ���H���̲ײ��ʤ�V
        bool is_positioned;																	// �O�_���H��w�I
        double HP;																			// ��q
        int is_killed_by;																	// �Q�ֱ���
        bool is_focused;																	// �O�_�Q��w
        static bool bitmapIsLoaded;															// bitmap�����O�_�w���J
        bool is_reporting;																	// �O�_���b�^������
        CAnimation man_dead_left, man_dead_right;
        CAnimation man_following_left, man_following_right;
        CAnimation flash, flash_multi, weakening;
        CAnimation blood;
        CAnimation man_following_girl_left, man_following_girl_right;
        CAnimation scoreReport;
        static CAnimation clicking;															// �m�ܥζq���ʵe
        static CMovingBitmap clicking_bar;													// �m�ܥζq��
        CPoint color_point[2];
    };

    class NormalMan : public Man
    {
    public:
        NormalMan(int x, int y, int start, int end, bool direction, int type);
        ~NormalMan();
        void LoadBitMap();
        void OnBeginState();																// ��l�ƪ��A
    private:
        // �Ω��x�s��l���A
        const int type;																		// �H������
        const int x;																		// �H��X�y�� (�a�Ϯy��)
        const int y;																		// �H��Y�y�� (�a�Ϯy��)
        const int start;																	// ���ʪ�����d����� X�b
        const int end;																		// ���ʪ��k��d����� X�b
        const bool direction;																// ���ʤ�V
    };

    class SpecialMan : public Man
    {
    public:
        SpecialMan(int x, int y, int start, int end, bool direction, int type);
        ~SpecialMan();
        void LoadBitMap();
        void OnBeginState();																// ��l�ƪ��A
    private:
        // type == 1 => Blue hair
        // type == 2 => Brown hair
        // type == 3 => Gold hair
        // �Ω��x�s��l���A
        const int type;																		// �H������
        const int x;																		// �H��X�y�� (�a�Ϯy��)
        const int y;																		// �H��Y�y�� (�a�Ϯy��)
        const int start;																	// ���ʪ�����d����� X�b
        const int end;																		// ���ʪ��k��d����� X�b
        const bool direction;																// ���ʤ�V
    };
}