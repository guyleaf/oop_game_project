#include <vector>
namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        ~MainGirl();

        void OnBeginState();											// ��l�ƪ��A
        void LoadBitMap();												// ���J�Ϥ��ʵe
        void OnMouseMove(CPoint point);									// �B�z�ƹ�����
        int GetCursorX();												// ���o�ƹ�X�y�� (�����y��)
        int GetCursorY();												// ���o�ƹ�Y�y�� (�����y��)
        int GetPositionX();												// ���o�k�D����mX�y�� (�a�Ϯy��)
        int GetPositionY();												// ���o�k�D����mY�y�� (�a�Ϯy��)
        int Height();													// �k�D������
        int Width();													// �k�D���e��
        void OnMove(CGameMap* map, UI* ui);								// �B�z�ʧ@��s�޿�
        void OnShow(CGameMap* map, UI* ui);								// �B�z����޿�
        void Attack(Man* man, CGameMap* map);							// ����
        void ShowFocus();												// ��ܷǬP
        void SetIsFocusing(bool status);								// �]�w��w���A (��H)
        void SetFocusPerson(CGameMap* map, Man* man);					// �]�w��w���k��
        void SetIsAttacking(bool status);								// �]�w�������A
        void SetIsLocked(bool status);									// �]�w�ꦺ���A (�P��L�k�ͷm)
        void SetIsReinforced(bool status);								// �]�w�S��ɶ��Ҧ�
        bool IsInAnimation();											// �O�_���b�L���ʵe
        bool IsReinforced();											// �O�_�O�S��ɶ��Ҧ�
        bool IsLocked();												// �O�_�ꦺ
        bool IsFocusing();												// �O�_��w
        bool IsFocusPerson(Man* man);									// �O�_����w��H
        bool IsAttacking();												// �O�_���b����
        bool IsReporting();												// �O�_���b�������
        void Lose();													// �鱼
        void Click();													// �ꦺ���A�ɥͮġA�C���@�������@��
        bool IsClicked();												// ��w���A�ɥͮġA�O�_���U�ƹ�
        void AddSlave(Man* man);										// �N�m�쪺�k�͡A�[�J������C
    private:
        void InitializeReinforcing();									// ��l�ƯS��ɶ��Ѽ�
        void SetVelocity(CGameMap* map);								// �]�w���ʳt��
        void DrawBeam(CGameMap* map);									// �e�X�p�g
        int x, y;														// �k�D����m (�a�Ϯy��)
        int cursor_x, cursor_y;											// �ƹ���Ц�m (�����y��)
        bool moving;													//�O�_���b����
        bool direction;													//false => ����, true => ���k
        int velocity;													// �t��
        bool is_focusing;												// ��w���A
        bool is_attacking;												// �������A
        bool is_locked;													// �ꦺ���A (�P��L�k�ͷm)
        bool is_clicked;												// �ƹ����U (�P��L�k�ͷm)
        bool is_interrupted;											// �Ω�ǬP��ܡA�O�_�Q���_
        int focus_id;													//��w��H��ID
        bool is_bump;													// �������A
        bool is_reinforced;												// �S��ɶ����A
        bool is_reporting;												// ���⪬�A
        int state;														// ���A�A���`and�L���ʵe
        int delay_counter;												// �S��ɶ��P��S�ĩ���
        CAnimation girl_walk_left, girl_walk_right;
        CAnimation girl_run_left, girl_run_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
        CMovingBitmap girl_left_focusing_behind, girl_left_focusing_front;
        CMovingBitmap girl_right_focusing_behind, girl_right_focusing_front;
        CAnimation girl_right_reinforcing, girl_left_reinforcing;		// �S��ɶ��L���ʵe
        CAnimation focus_point_on, focus_point_off;						// �ǬP�ʵe
        CAnimation bump_left, bump_right;								// �����ʵe
        CAnimation reinforcing[2];										// �S��ɶ��ʵe
        CBitmap m_memBitmap;											// �ܨ������Ϥ�
        CPoint beam_pos[4];												// �p�g�y��
        vector<Man*> slaves;
        CAnimation surprising_left, surprising_right;
        CAnimation fun;
    };
}


