#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateInit.h"
#include <string>

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲開頭畫面物件
    /////////////////////////////////////////////////////////////////////////////

    CGameStateInit::CGameStateInit(CGame* g, int* score) : CGameState(g, score)
    {
        change = false;
        changeState = false;
        delay_counter = 72;
        view = 0;
        now = 0;
        cursor_x1 = cursor_y1 = -1;
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
        about_button.LoadBitmap(IDB_ABOUT_BUTTON);
        about_button_hover.LoadBitmap(IDB_ABOUT_BUTTON_HOVER);
        about.LoadBitmap(IDB_ABOUT);
        intro_button.LoadBitmap(IDB_INTRO_BUTTON);
        intro_button_hover.LoadBitmap(IDB_INTRO_BUTTON_HOVER);
        backInit.LoadBitmap(IDB_BUTTON1_1);
        backInit_hover.LoadBitmap(IDB_BUTTON1_2);
        next.LoadBitmap("RES/init/next.bmp", RGB(255, 255, 255));
        before.LoadBitmap("RES/init/before.bmp", RGB(255, 255, 255));
        CAudio::Instance()->Load(AUDIO_INIT, "sounds\\init.mp3");
        CAudio::Instance()->Load(AUDIO_PRESS, "sounds\\press.mp3");
        CAudio::Instance()->Load(AUDIO_GAME, "sounds\\game.mp3");
        CAudio::Instance()->Play(AUDIO_INIT, true);
        ///////////////////////////////////////////////////////////////
        char text[150] = { 0 };

        for (int i = 1; i <= 28; i++)
        {
            strcpy(text, ("RES/init/intro/intro1/intro1 (" + to_string(i) + ").bmp").c_str());
            intro[0].AddBitmap(text);
        }

        for (int i = 1; i <= 22; i++)
        {
            strcpy(text, ("RES/init/intro/intro2/intro2 (" + to_string(i) + ").bmp").c_str());
            intro[1].AddBitmap(text);
        }

        for (int i = 1; i <= 14; i++)
        {
            strcpy(text, ("RES/init/intro/intro3/intro3 (" + to_string(i) + ").bmp").c_str());
            intro[2].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/init/intro/intro4/intro4 (" + to_string(i) + ").bmp").c_str());
            intro[3].AddBitmap(text);
        }

        for (int i = 1; i <= 15; i++)
        {
            strcpy(text, ("RES/init/intro/intro5/intro5 (" + to_string(i) + ").bmp").c_str());
            intro[4].AddBitmap(text);
        }

        for (int i = 0; i < 5; i++)
        {
            intro[i].SetDelayCount(5);
        }
    }

    void CGameStateInit::OnBeginState()
    {
        waveOutGetVolume(0, &volume);

        if (volume == 0)
        {
            volume = 0xFFFFFFFF;
            //waveOutSetVolume(0, volume);
            change = true;
        }
        else
        {
            change = false;
        }

        if (*score != -1)
        {
            CAudio::Instance()->Play(AUDIO_INIT, true);
            *score = -1;
        }

        changeState = false;
        delay_counter = 72;
        cursor_x1 = cursor_y1 = -1;
        /*for (int i = 0; i < 5; i++)
        {
        	intro[i].Reset();
        }*/
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
        if (view == 0)
        {
            if (changeState && --delay_counter <= 0)
                GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN

            if (cursor_x1 >= 180 && cursor_x1 <= 375)
            {
                if (cursor_y1 >= 490 && cursor_y1 <= 540)
                {
                    if (!changeState)
                    {
                        CAudio::Instance()->Play(AUDIO_PRESS, false);
                        CAudio::Instance()->Stop(AUDIO_INIT);
                        CAudio::Instance()->Play(AUDIO_GAME, false);
                    }

                    changeState = true;
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
                        waveOutSetVolume(0, volume);
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        if (view == 0)
        {
            if (cursor_x1 >= 640 && cursor_x1 <= 695)
            {
                if (cursor_y1 >= 390 && cursor_y1 <= 435)
                {
                    view = 1;
                }
            }

            if (cursor_x1 >= 640 && cursor_x1 <= 695)
            {
                if (cursor_y1 >= 320 && cursor_y1 <= 365)
                {
                    view = 2;
                }
            }
        }

        if (view == 1)
        {
            if (cursor_x1 >= 50 && cursor_x1 <= 80)
            {
                if (cursor_y1 >= 300 && cursor_y1 <= 340)
                {
                    GotoGameState(GAME_STATE_INIT);
                    view = 0;
                }
            }
        }

        if (view == 2)
        {
            intro[now].OnMove();

            if (cursor_x1 >= 720 && cursor_x1 <= 770)
            {
                if (cursor_y1 >= 310 && cursor_y1 <= 370)
                {
                    if (now == 3)
                    {
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                        now = 4;
                    }

                    if (now == 2)
                    {
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                        now = 3;
                    }

                    if (now == 1)
                    {
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                        now = 2;
                    }

                    if (now == 0)
                    {
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                        now = 1;
                    }
                }
            }

            if (cursor_x1 >= 35 && cursor_x1 <= 75)
            {
                if (cursor_y1 >= 310 && cursor_y1 <= 370)
                {
                    if (now == 1)
                    {
                        now = 0;
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }

                    if (now == 2)
                    {
                        now = 1;
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }

                    if (now == 3)
                    {
                        now = 2;
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }

                    if (now == 4)
                    {
                        now = 3;
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }
                }
            }

            if (cursor_x1 >= 300 && cursor_x1 <= 495)
            {
                if (cursor_y1 >= 530 && cursor_y1 <= 580)
                {
                    GotoGameState(GAME_STATE_INIT);
                    view = 0;
                }
            }

            if (change == false)
            {
                if (cursor_x1 >= 580 && cursor_x1 <= 640)
                {
                    if (cursor_y1 >= 520 && cursor_y1 <= 580)
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
                if (cursor_x1 >= 580 && cursor_x1 <= 640)
                {
                    if (cursor_y1 >= 520 && cursor_y1 <= 580)
                    {
                        change = false;
                        waveOutSetVolume(0, volume);
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }
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
        //int i = 0;
        if (changeState)
        {
            CDDraw::BltBackColor(RGB(0, 0, 0));
            return;
        }

        //
        // 貼上logo
        //
        //i = false;
        logo.SetTopLeft((SIZE_X - logo.Width()) / 2, (SIZE_Y - logo.Height()) / 8);
        logo.ShowBitmap();
        button1_1.SetTopLeft(180, 490);
        button1_1.ShowBitmap();
        button2_1.SetTopLeft(420, 490);
        button2_1.ShowBitmap();
        voice1.SetTopLeft(630, 450);
        voice1.ShowBitmap();
        about_button.SetTopLeft(640, 390);
        about_button.ShowBitmap();
        intro_button.SetTopLeft(640, 320);
        intro_button.ShowBitmap();

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

        if (cursor_x2 >= 640 && cursor_x2 <= 695)
        {
            if (cursor_y2 >= 390 && cursor_y2 <= 435)
            {
                about_button_hover.SetTopLeft(640, 390);
                about_button_hover.ShowBitmap();
            }
        }

        if (cursor_x2 >= 640 && cursor_x2 <= 695)
        {
            if (cursor_y2 >= 320 && cursor_y2 <= 365)
            {
                intro_button_hover.SetTopLeft(640, 320);
                intro_button_hover.ShowBitmap();
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

        ////////////////////////////////////////////////////////////////////////

        if (view == 1)
        {
            about.SetTopLeft((SIZE_X - about.Width()) / 2, (SIZE_Y - about.Height()) / 8);
            about.ShowBitmap();
            before.SetTopLeft(50, 300);
            before.ShowBitmap();
        }

        if (view == 2)
        {
            intro[now].SetTopLeft(0, 0);
            intro[now].OnShow();
            backInit.SetTopLeft(300, 530);
            backInit.ShowBitmap();

            if (cursor_x2 >= 35 && cursor_x2 <= 75)
            {
                if (cursor_y2 >= 310 && cursor_y2 <= 370)
                {
                    if (now > 0)
                    {
                        before.SetTopLeft(35, 310);
                        before.ShowBitmap();
                    }
                }
            }

            if (cursor_x2 >= 720 && cursor_x2 <= 770)
            {
                if (cursor_y2 >= 310 && cursor_y2 <= 370)
                {
                    if (now < 4)
                    {
                        next.SetTopLeft(720, 310);
                        next.ShowBitmap();
                    }
                }
            }

            if (cursor_x2 >= 300 && cursor_x2 <= 495)
            {
                if (cursor_y2 >= 530 && cursor_y2 <= 580)
                {
                    backInit_hover.SetTopLeft(300, 530);
                    backInit_hover.ShowBitmap();
                }
            }

            if (change == false)
            {
                voice1.SetTopLeft(580, 520);
                voice1.ShowBitmap();

                if (cursor_x2 >= 580 && cursor_x2 <= 640)
                {
                    if (cursor_y2 >= 520 && cursor_y2 <= 580)
                    {
                        voice2.SetTopLeft(580, 520);
                        voice2.ShowBitmap();
                    }
                }
            }

            if (change == true)
            {
                voice4.SetTopLeft(580, 520);
                voice4.ShowBitmap();

                if (cursor_x2 >= 580 && cursor_x2 <= 640)
                {
                    if (cursor_y2 >= 520 && cursor_y2 <= 580)
                    {
                        voice3.SetTopLeft(580, 520);
                        voice3.ShowBitmap();
                    }
                }
            }
        }

        //
        // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
        //
        /*CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
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
        //pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        //CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
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
