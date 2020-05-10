#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateInit.h"

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲開頭畫面物件
    /////////////////////////////////////////////////////////////////////////////

    CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
    {
		change = false;
    }

    void CGameStateInit::OnInit()
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(0);	// 一開始的loading進度為0%
        //
        // 開始載入資料
        //
		//CAudio::Instance()->Load(AUDIO_INIT, "sounds\\init.mp3");
        logo.LoadBitmap(IDB_INITSCREEN);
		button1_1.LoadBitmap(IDB_BUTTON1_1);
		button1_2.LoadBitmap(IDB_BUTTON1_2);
		button2_1.LoadBitmap(IDB_BUTTON2_1);
		button2_2.LoadBitmap(IDB_BUTTON2_2);
		voice1.LoadBitmap(IDB_VOICE1);
		voice2.LoadBitmap(IDB_VOICE2);
		voice3.LoadBitmap(IDB_VOICE3);
		voice4.LoadBitmap(IDB_VOICE4);
		
        Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //
        // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
        //
    }
	
    void CGameStateInit::OnBeginState()
    {
		//CAudio::Instance()->Play(AUDIO_INIT, true);
    }

    void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_ESC = 27;
        const char KEY_SPACE = ' ';

        if (nChar == KEY_SPACE)
            GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
        else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
    }

    void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
    {

    }

	void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)
	{
		cursor_x1 = point.x;
		cursor_y1 = point.y;
	}

	void CGameStateInit::OnMove()
	{
		if (cursor_x1 >= 180 && cursor_x1 <= 375)
		{
			if (cursor_y1 >= 490 && cursor_y1 <= 540)
			{
				GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
			}
		}
		
		if (cursor_x1 >= 420 && cursor_x1 <= 615)
		{
			if (cursor_y1 >= 490 && cursor_y1 <= 540)
			{
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
			}
		}
		if (change == false)
		{
			if (cursor_x1 >= 630 && cursor_x1 <= 690)
			{
				if (cursor_y1 >= 450 && cursor_y1 <= 510)
				{
					change = true;
					waveOutGetVolume(0, &volume);
					waveOutSetVolume(0, 0);
					cursor_x1 = -1;
					cursor_y1 = -1;
				}
			}
		}
		else
		{
			if (cursor_x1 >= 630 && cursor_x1 <= 690)
			{
				if (cursor_y1 >= 450 && cursor_y1 <= 510)
				{
					change = false;
					waveOutSetVolume(0,volume);
					cursor_x1 = -1;
					cursor_y1 = -1;
				}
			}
		}


		

	}
	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
	{
		cursor_x2 = point.x;
		cursor_y2 = point.y;
	}


    void CGameStateInit::OnShow()
    {
        //
        // 貼上logo
        //
		//i = false;
        logo.SetTopLeft((SIZE_X - logo.Width()) / 2, (SIZE_Y - logo.Height()) / 8);
        logo.ShowBitmap();
		button1_1.SetTopLeft(180,490);
		button1_1.ShowBitmap();
		button2_1.SetTopLeft(420, 490);
		button2_1.ShowBitmap();
		voice1.SetTopLeft(630, 450);
		voice1.ShowBitmap();
		if (cursor_x2 >= 180 && cursor_x2 <= 375)
		{
			if (cursor_y2 >= 490 && cursor_y2 <= 540)
			{
				button1_2.SetTopLeft(180, 490);
				button1_2.ShowBitmap();
			}
		}

		if (cursor_x2 >= 420 && cursor_x2 <= 615)
		{
			if (cursor_y2 >= 490 && cursor_y2 <= 540)
			{
				button2_2.SetTopLeft(420, 490);
				button2_2.ShowBitmap();
			}
		}
		////////////////////////////////////////////////////////////////////
		
		if (change == false)
		{
			voice1.SetTopLeft(630, 450);
			voice1.ShowBitmap();
			if (cursor_x2 >= 630 && cursor_x2 <= 690)
			{
				if (cursor_y2 >= 450 && cursor_y2 <= 510)
				{
					voice2.SetTopLeft(630, 450);
					voice2.ShowBitmap();
				}
			}
				
		}
		if (change == true)
		{
			
			voice4.SetTopLeft(630, 450);
			voice4.ShowBitmap();
			if (cursor_x2 >= 630 && cursor_x2 <= 690)
			{
				if (cursor_y2 >= 450 && cursor_y2 <= 510)
				{
					voice3.SetTopLeft(630, 450);
					voice3.ShowBitmap();
				}
			}

			
		}

        //
        // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
        //
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CFont f, *fp;
        f.CreatePointFont(140, "Consolas");	// 產生 font f; 160表示16 point的字
        fp = pDC->SelectObject(&f);					// 選用 font f
      /*  pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(255, 105, 180));
       // pDC->TextOut(280, 490, "點擊任意處開始遊戲");
       // pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
        if (ENABLE_GAME_PAUSE)
            pDC->TextOut(295, 525, "按下 Ctrl-Q 暫停");*/
      //  pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }

	/*int CGameStateInit::GetCursorX1()
	{
		return cursor_x1;
	}
	int CGameStateInit::GetCursorY1()
	{
		return cursor_y1;
	}
	int CGameStateInit::GetCursorX2()
	{
		return cursor_x2;
	}
	int CGameStateInit::GetCursorY2()
	{
		return cursor_y2;
	}*/
}
