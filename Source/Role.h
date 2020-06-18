namespace game_framework
{
    class Role
    {
    public:
        int GetX();																			// ���o�H��X�y�� (�a�Ϯy��)
        int GetY();																			// ���o�H��Y�y�� (�a�Ϯy��)
        int GetWidth();																		// ���o�H���e��
        int GetHeight();																	// ���o�H������
    protected:
        int x, y;																			// �H����m
        bool moving;																		// �O�_���b����
        bool direction;																		// false => ����, true => ���k
        int velocity;																		// ���ʳt��
        int range[2];																		// ���ʽd��
        int distance;																		// ���}�ɪ��Ѿl�Z��
        int status;																			// ���A
        CAnimation left, right;
        CMovingBitmap left_stand, right_stand;
    };
}


