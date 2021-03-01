namespace game_framework
{
    class Girl : public Role
    {
    public:
        Girl();
        virtual ~Girl();
        virtual void LoadBitMap() = 0;
        virtual void OnBeginState() = 0;													// ��l�ƪ��A
        void SetDirection(bool direction);													// �]�w���ʤ�V
        void OnMove(CGameMap* map, int seed);												// �B�z�ʧ@��s�޿�
        void OnShow(CGameMap* map);															// �B�z����޿�
        void SetIsShocking(bool status);													// �]�w�O�_�`�N��k�D�����b�y�b�k��
        bool IsShocking();																	// �O�_�`�N��k�D�����b�y�b�k��
        void LockPerson(Man* man, CGameMap* map);											// ��w�k��
        void Attack(Man* man, CGameMap* map);												// �����k��
        bool IsLocked();																	// �O�_��w
        void Win();																			// Ĺ
        void Lose();																		// ��
        bool IsAlive();																		// �O�_�s��
        bool IsAlreadyDead();																// �O�_�w�g���z
        virtual bool IsSpecialGirl() = 0;													// �O�_�����v�S��k��
    protected:
        void DrawBeam(CGameMap* map);														// �e�X�p�g����
        bool is_shocking;																	// �O�_�`�N��k�D�����b�y�b�k��
        int damage;																			// �����O
        CPoint beam_pos[4];																	// �p�g������m
        CAnimation exclamation;
        CAnimation surprising_left, surprising_right;
        CMovingBitmap shooting_left, shooting_right;
        CMovingBitmap notice_left, notice_right;
        CAnimation flying_left, flying_right;
        CAnimation leaving_left, leaving_right;
        CAnimation fun;
    };

    class NormalGirl : public Girl
    {
    public:
        NormalGirl(int x, int y, int start, int end, bool direction, int type);
        ~NormalGirl();
        void LoadBitMap();
        void OnBeginState();																// ��l�ƪ��A
        bool IsSpecialGirl();																// �O�_�����v�S��k��
    private:
        // �Ω��x�s��l���A
        const int type;																		// �H������
        const int x;																		// �H��X�y�� (�a�Ϯy��)
        const int y;																		// �H��Y�y�� (�a�Ϯy��)
        const int start;																	// ���ʪ�����d����� X�b
        const int end;																		// ���ʪ��k��d����� X�b
        const bool direction;																// ���ʤ�V
    };

    class SpecialGirl : public Girl
    {
    public:
        SpecialGirl(int x, int y, int start, int end, bool direction, int type);
        ~SpecialGirl();
        void LoadBitMap();
        void OnBeginState();
        bool IsSpecialGirl();
    private:
        const int type;																		// �H������
        const int x;																		// �H��X�y�� (�a�Ϯy��)
        const int y;																		// �H��Y�y�� (�a�Ϯy��)
        const int start;																	// ���ʪ�����d����� X�b
        const int end;																		// ���ʪ��k��d����� X�b
        const bool direction;																// ���ʤ�V
    };
}