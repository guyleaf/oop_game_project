#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateRun.h"


namespace game_framework
{

    /////////////////////////////////////////////////////////////////////////////
    // Constants
    /////////////////////////////////////////////////////////////////////////////

    enum AUDIO_ID  				// �w�q�U�ح��Ī��s��
    {
        AUDIO_DING,				// 0
        AUDIO_LAKE,				// 1
        AUDIO_NTUT				// 2
    };

    /////////////////////////////////////////////////////////////////////////////
    // �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
    /////////////////////////////////////////////////////////////////////////////

    CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
    {
    }

    CGameStateRun::~CGameStateRun()
    {
    }

    void CGameStateRun::OnBeginState()
    {
        const int HITS_LEFT = 10;
        const int HITS_LEFT_X = 590;
        const int HITS_LEFT_Y = 0;
        /*hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��*/
        CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
        CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
        CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
    }

    void CGameStateRun::OnMove()							// ���ʹC������
    {
        //
        // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
        //
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        girl.OnMove(&map);
    }

    void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
    {
        //
        // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
        //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
        //
        ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
        //
        // �}�l���J���
        //
        map.LoadBitMap();
        girl.LoadBitMap();
        //
        // ��������Loading�ʧ@�A�����i��
        //
        ShowInitProgress(50);
        Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
        CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
        CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
        CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
        //
        // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
        //
    }

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    }

    void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    }

    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
        girl.OnMouseMove(&map, point);
    }

    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {
    }

    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
    }

    void CGameStateRun::OnShow()
    {
        //
        //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
        //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
        //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
        //
        //
        //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
        //
        map.OnShow(1);
        girl.OnShow(&map);
    }
}