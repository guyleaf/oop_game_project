#include <string>
namespace game_framework
{
    class CGameMap
    {
    public:
        CGameMap();
        void LoadBitMap();
        void OnBeginState();												// ��l�ƪ��A
        void OnMove();
        void OnShow();
        void Addsx(int val);												// ���ʦa��X��m
        void Addsy(int val);												// ���ʦa��Y��m
        int Height();														// ���o����
        int Width();														// ���o�e��
        int ScreenX(int val);												// �ഫ�a��X�y�Цܵ���X�y��
        int ScreenY(int val);												// �ഫ�a��Y�y�Цܵ���Y�y��
        bool IsMapChanging();												// �O�_�a�ϥ��b����
        void SetLevel(int level);											// �]�w�Ӽh
        int GetLevel();														// ���o�Ӽh
        bool IsInScreen(int start_x, int end_x);							// �O�_�b�����d���
        bool IsEmpty(int x, int y);											// �O�_�Ӯy�Ь���
    private:
        CMovingBitmap firstFloor, secondFloor, thirdFloor, ceiling;
        int sx, sy;															// �a�Ϯy��
        int level;															// �Ӽh
        int height, width;													// ����, �e��
        int left_edge, right_edge;											// ����ɽu, �k��ɽu
        int counter;														// �a���������
        bool is_mapChanging;												// �O�_�a�ϥ��b����
    };
}


