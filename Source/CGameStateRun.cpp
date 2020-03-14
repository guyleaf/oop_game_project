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

    enum AUDIO_ID  				// 定義各種音效的編號
    {
        AUDIO_DING,				// 0
        AUDIO_LAKE,				// 1
        AUDIO_NTUT				// 2
    };

    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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

        for (int i = 0; i < NUMBALLS; i++)  				// 設定球的起始座標
        {
            int x_pos = i % BALL_PER_ROW;
            int y_pos = i / BALL_PER_ROW;
            ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
            ball[i].SetDelay(x_pos);
            ball[i].SetIsAlive(true);
        }

        eraser.Initialize();
        background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
        help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
        hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
        CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
        CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
        CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
    }

    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        //
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        //
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        //
        // 移動背景圖的座標
        //
        if (background.Top() > SIZE_Y)
            background.SetTopLeft(60, -background.Height());

        background.SetTopLeft(background.Left(), background.Top() + 1);
        //
        // 移動球
        //
        int i;

        for (i = 0; i < NUMBALLS; i++)
            ball[i].OnMove();

        //
        // 移動擦子
        //
        eraser.OnMove();

        //
        // 判斷擦子是否碰到球
        //
        for (i = 0; i < NUMBALLS; i++)
            if (ball[i].IsAlive() && ball[i].HitEraser(&eraser))
            {
                ball[i].SetIsAlive(false);
                CAudio::Instance()->Play(AUDIO_DING);
                hits_left.Add(-1);

                //
                // 若剩餘碰撞次數為0，則跳到Game Over狀態
                //
                if (hits_left.GetInteger() <= 0)
                {
                    CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
                    CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
                    GotoGameState(GAME_STATE_OVER);
                }
            }

        //
        // 移動彈跳的球
        //
        bball.OnMove();
    }

    void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
        //
        // 開始載入資料
        //
        int i;

        for (i = 0; i < NUMBALLS; i++)
            ball[i].LoadBitmap();								// 載入第i個球的圖形

        eraser.LoadBitmap();
        background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
        //
        // 完成部分Loading動作，提高進度
        //
        ShowInitProgress(50);
        Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //
        // 繼續載入其他資料
        //
        help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
        corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
        corner.ShowBitmap(background);							// 將corner貼到background
        bball.LoadBitmap();										// 載入圖形
        hits_left.LoadBitmap();
        CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
        CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
        CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
        //
        // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
        //
    }

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭

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
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭

        if (nChar == KEY_LEFT)
            eraser.SetMovingLeft(false);

        if (nChar == KEY_RIGHT)
            eraser.SetMovingRight(false);

        if (nChar == KEY_UP)
            eraser.SetMovingUp(false);

        if (nChar == KEY_DOWN)
            eraser.SetMovingDown(false);
    }

    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
        eraser.SetMovingLeft(true);
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        eraser.SetMovingLeft(false);
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
    }

    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
        eraser.SetMovingRight(true);
    }

    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        eraser.SetMovingRight(false);
    }

    void CGameStateRun::OnShow()
    {
        //
        //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
        //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
        //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
        //
        //
        //  貼上背景圖、撞擊數、球、擦子、彈跳的球
        //
        background.ShowBitmap();			// 貼上背景圖
        help.ShowBitmap();					// 貼上說明圖
        hits_left.ShowBitmap();

        for (int i = 0; i < NUMBALLS; i++)
            ball[i].OnShow();				// 貼上第i號球

        bball.OnShow();						// 貼上彈跳的球
        eraser.OnShow();					// 貼上擦子
        //
        //  貼上左上及右下角落的圖
        //
        corner.SetTopLeft(0, 0);
        corner.ShowBitmap();
        corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
        corner.ShowBitmap();
    }
}