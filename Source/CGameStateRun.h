#include "GameObject.h"
#include <vector>

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
    // �C��Member function��Implementation���n����
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateRun : public CGameState
    {
    public:
        CGameStateRun(CGame* g, int* score, bool* isDead);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        void GenerateGoldBoy(int level, bool direction, bool top);	// �ͦ����v�k�ͻP�k��
        void LoadData();											// Ū���~������ɮרø��J
        void ChangeGameState(int state);							// ����GameState
        CGameMap map;
        MainGirl* mainGirl;
        vector<Man*> man[4][2];
        vector<Girl*> girl[4][2];
        vector<Heart*> hearts;
        vector<Girl*> girlsOnScreen;
        UI ui;
        Teacher* teacher;
        bool isGoldBoyShowUp;
    };
}

