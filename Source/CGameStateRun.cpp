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

    CGameStateRun::CGameStateRun(CGame* g) : CGameState(g), NUMBALLS(28)
    {
        ball = new CBall[NUMBALLS];
    }

    CGameStateRun::~CGameStateRun()
    {
        delete[] ball;
    }

    void CGameStateRun::OnBeginState()
    {
        const int BALL_GAP = 90;
        const int BALL_XY_OFFSET = 45;
        const int BALL_PER_ROW = 7;
        const int HITS_LEFT = 10;
        const int HITS_LEFT_X = 590;
        const int HITS_LEFT_Y = 0;
        const int BACKGROUND_X = 60;
        const int ANIMATION_SPEED = 15;

        for (int i = 0; i < NUMBALLS; i++)  				// �]�w�y���_�l�y��
        {
            int x_pos = i % BALL_PER_ROW;
            int y_pos = i / BALL_PER_ROW;
            ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
            ball[i].SetDelay(x_pos);
            ball[i].SetIsAlive(true);
        }

        eraser.Initialize();
        background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
        help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
        hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
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
        //
        // ���ʭI���Ϫ��y��
        //
        if (background.Top() > SIZE_Y)
            background.SetTopLeft(60, -background.Height());

        background.SetTopLeft(background.Left(), background.Top() + 1);
        //
        // ���ʲy
        //
        int i;

        for (i = 0; i < NUMBALLS; i++)
            ball[i].OnMove();

        //
        // �������l
        //
        eraser.OnMove();

        //
        // �P�_���l�O�_�I��y
        //
        for (i = 0; i < NUMBALLS; i++)
            if (ball[i].IsAlive() && ball[i].HitEraser(&eraser))
            {
                ball[i].SetIsAlive(false);
                CAudio::Instance()->Play(AUDIO_DING);
                hits_left.Add(-1);

                //
                // �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
                //
                if (hits_left.GetInteger() <= 0)
                {
                    CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
                    CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
                    GotoGameState(GAME_STATE_OVER);
                }
            }

        //
        // ���ʼu�����y
        //
        bball.OnMove();
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
        int i;

        for (i = 0; i < NUMBALLS; i++)
            ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�

        eraser.LoadBitmap();
        background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
        //
        // ��������Loading�ʧ@�A�����i��
        //
        ShowInitProgress(50);
        Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
        //
        // �~����J��L���
        //
        help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
        corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
        corner.ShowBitmap(background);							// �Ncorner�K��background
        bball.LoadBitmap();										// ���J�ϧ�
        hits_left.LoadBitmap();
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

        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(true);

        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(true);

        if (nChar == KEY_UP)
            eraser.SetMovingUp(true);

        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(true);
    }

    void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y

        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(false);

        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(false);

        if (nChar == KEY_UP)
            eraser.SetMovingUp(false);

        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(false);
    }

    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {
        eraser.SetMovingLeft(true);
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        eraser.SetMovingLeft(false);
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
    }

    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {
        eraser.SetMovingRight(true);
    }

    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        eraser.SetMovingRight(false);
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
        background.ShowBitmap();			// �K�W�I����
        help.ShowBitmap();					// �K�W������
        hits_left.ShowBitmap();

        for (int i = 0; i < NUMBALLS; i++)
            ball[i].OnShow();				// �K�W��i���y

        bball.OnShow();						// �K�W�u�����y
        eraser.OnShow();					// �K�W���l
        //
        //  �K�W���W�Υk�U��������
        //
        corner.SetTopLeft(0, 0);
        corner.ShowBitmap();
        corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
        corner.ShowBitmap();
    }
}