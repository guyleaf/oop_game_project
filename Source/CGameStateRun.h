#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include "CGameMap.h"
#include "MainGirl.h"
#include "Man.h"

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
    // �C��Member function��Implementation���n����
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateRun : public CGameState
    {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap	background;	// �I����
        CMovingBitmap	help;		// ������
        CInteger		hits_left;	// �ѤU��������
        CGameMap map;
        MainGirl girl;
		Man man;
    };
}

