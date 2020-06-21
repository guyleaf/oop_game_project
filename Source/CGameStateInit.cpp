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

    CGameStateInit::CGameStateInit(CGame* g, int* score, bool* isDead) : CGameState(g, score, isDead) // 初始化
    {
        change = false;
        changeState = false;
        delay_counter = 72;
        view = 0;
        now = 0;
        cursor_x1 = cursor_y1 = -1;
        isLoaded = false;
        isPlaying = false;
    }

    void CGameStateInit::OnInit()
    {
        ShowInitProgress(0); // 一開始的loading進度為0%
		logo.LoadBitmap(IDB_INITSCREEN); // 開始載入需要的圖片素材及音效
        button1_1.LoadBitmap(IDB_BUTTON1_1);
        button1_2.LoadBitmap(IDB_BUTTON1_2);
        button2_1.LoadBitmap(IDB_BUTTON2_1);
        button2_2.LoadBitmap(IDB_BUTTON2_2);
        voice1.LoadBitmap(IDB_VOICE1);
        voice2.LoadBitmap(IDB_VOICE2);
        voice3.LoadBitmap(IDB_VOICE3);
        voice4.LoadBitmap(IDB_VOICE4);
        init_voice1.LoadBitmap("RES/init/init_voice1.bmp", RGB(255, 255, 255));
        init_voice2.LoadBitmap("RES/init/init_voice2.bmp", RGB(255, 255, 255));
        init_voice3.LoadBitmap("RES/init/init_voice3.bmp", RGB(255, 255, 255));
        init_voice4.LoadBitmap("RES/init/init_voice4.bmp", RGB(255, 255, 255));
        about_button.LoadBitmap(IDB_ABOUT_BUTTON);
        about_button_hover.LoadBitmap(IDB_ABOUT_BUTTON_HOVER);
        about.LoadBitmap(IDB_ABOUT);
        intro_button.LoadBitmap(IDB_INTRO_BUTTON);
        intro_button_hover.LoadBitmap(IDB_INTRO_BUTTON_HOVER);
        backInit.LoadBitmap("RES/init/back1.bmp", RGB(255, 255, 255));
        backInit_hover.LoadBitmap("RES/init/back2.bmp", RGB(255, 255, 255));
        next.LoadBitmap("RES/init/next.bmp", RGB(255, 255, 255));
        before.LoadBitmap("RES/init/before.bmp", RGB(255, 255, 255));
        before_hover.LoadBitmap("RES/init/before_hover.bmp", RGB(255, 255, 255));
        CAudio::Instance()->Load(AUDIO_INIT, "sounds\\init.mp3");
        CAudio::Instance()->Load(AUDIO_PRESS, "sounds\\press.mp3");
        CAudio::Instance()->Load(AUDIO_GAME, "sounds\\game.mp3");
        CAudio::Instance()->Play(AUDIO_INIT, true);
        isPlaying = true; // 判斷音樂播放設為True
        char text[150] = { 0 };

        for (int i = 1; i <= 28; i++) // 用for迴圈載入一組一組的動畫圖
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

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/init/intro/intro6/intro6 (" + to_string(i) + ").bmp").c_str());
            intro[5].AddBitmap(text);
        }

        for (int i = 0; i < 5; i++)
        {
            intro[i].SetDelayCount(4);
        }

        isLoaded = true;
    }

    void CGameStateInit::OnBeginState()
    {
        waveOutGetVolume(0, &volume); // 音樂控制

        if (volume == 0)
        {
            volume = 0xFFFFFFFF;
            change = true; // true為靜音
        }
        else
        {
            change = false;	// false為開啟聲音
        }

        *score = -1; // 分數重置
        changeState = false; // false為還未切換狀態
        delay_counter = 72;
        cursor_x1 = cursor_y1 = -1;	// 將鼠標位置重置

        if (isLoaded && !isPlaying)
        {
            CAudio::Instance()->Play(AUDIO_INIT, true);

            for (int i = 0; i < 5; i++)
            {
                intro[i].Reset();
            }

            isPlaying = true;
        }
    }

    void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_ESC = 27;

        if (nChar == KEY_ESC) // 按ESC鍵關閉遊戲
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0); // 關閉遊戲
    }

    void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)	 // 定位鼠標左鍵動作位置
    {
        cursor_x1 = point.x;
        cursor_y1 = point.y;
    }

    void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// 定位鼠標滑過的動作位置
    {
        cursor_x2 = point.x;
        cursor_y2 = point.y;
    }

    void CGameStateInit::OnMove()
    {
        if (view == 0) // 以view變數來判斷現在狀態，0為初始畫面
        {
            if (changeState && --delay_counter <= 0) // 當切換狀態為true且黑屏延遲時間結束時
                GotoGameState(GAME_STATE_RUN); // 切換至GAME_STATE_RUN

            if (cursor_x1 >= 180 && cursor_x1 <= 375)
            {
                if (cursor_y1 >= 490 && cursor_y1 <= 540)
                {
                    if (!changeState) // 如果還未切換狀態完成
                    {
                        isPlaying = false;
                        CAudio::Instance()->Play(AUDIO_PRESS, false); // 播放按按鈕的音效
                        CAudio::Instance()->Stop(AUDIO_INIT); // 停止初始畫面的音樂
                        CAudio::Instance()->Play(AUDIO_GAME, false); // 播放遊戲進行中的音樂
                    }

                    changeState = true;
                }
            }

            if (cursor_x1 >= 420 && cursor_x1 <= 615)
            {
                if (cursor_y1 >= 490 && cursor_y1 <= 540)
                {
                    PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0); // 關閉遊戲
                }
            }

            // 聲音靜音按鈕部分

            if (change == false) // 沒靜音
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
            else // 靜音
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

            if (cursor_x1 >= 640 && cursor_x1 <= 695) // 如果點擊About按紐
            {
                if (cursor_y1 >= 390 && cursor_y1 <= 435)
                {
                    view = 1; // 將view切換到1
                }
            }

            if (cursor_x1 >= 640 && cursor_x1 <= 695) // 如果點擊遊戲說明按紐
            {
                if (cursor_y1 >= 320 && cursor_y1 <= 365)
                {
                    view = 2; // 將view切換到2
                }
            }
        }

        if (view == 1) // 當view為1時
        {
            if (cursor_x1 >= 50 && cursor_x1 <= 95)
            {
                if (cursor_y1 >= 300 && cursor_y1 <= 350)
                {
                    GotoGameState(GAME_STATE_INIT);	// 點擊返回鍵會切回初始畫面
                    view = 0; // view切回0
                }
            }
        }

        if (view == 2) // 當view為2時
        {
            intro[now].OnMove(); // 開始播放說明畫面

            if (cursor_x1 >= 720 && cursor_x1 <= 770) // 當點擊下一頁鍵時會切到下一個說明畫面
            {
                if (cursor_y1 >= 310 && cursor_y1 <= 370)
                {
                    if (now == 4)
                    {
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                        now = 5;
                    }

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

            if (cursor_x1 >= 35 && cursor_x1 <= 75)	// 當點擊上一頁鍵時會切到上一個說明畫面
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

                    if (now == 5)
                    {
                        now = 4;
                        cursor_x1 = -1;
                        cursor_y1 = -1;
                    }
                }
            }

            //聲音控制部分

            if (cursor_x1 >= 300 && cursor_x1 <= 495)
            {
                if (cursor_y1 >= 518 && cursor_y1 <= 570)
                {
                    GotoGameState(GAME_STATE_INIT); // 按下回主選單鍵回到初始畫面
                    view = 0; // view切回0
                }
            }

            if (change == false) // 沒靜音
            {
                if (cursor_x1 >= 580 && cursor_x1 <= 640)
                {
                    if (cursor_y1 >= 505 && cursor_y1 <= 570)
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
                if (cursor_x1 >= 580 && cursor_x1 <= 640) // 靜音
                {
                    if (cursor_y1 >= 505 && cursor_y1 <= 570)
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

    void CGameStateInit::OnShow()
    {
        if (changeState)
        {
            CDDraw::BltBackColor(RGB(0, 0, 0));
            return;
        }

        logo.SetTopLeft((SIZE_X - logo.Width()) / 2, (SIZE_Y - logo.Height()) / 8); // 將主畫面的背景與按紐圖片設置好位置
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
                button1_2.SetTopLeft(180, 490); // 如果滑鼠滑到開始遊戲的按鈕上會有hover的效果
                button1_2.ShowBitmap();
            }
        }

        if (cursor_x2 >= 420 && cursor_x2 <= 615)
        {
            if (cursor_y2 >= 490 && cursor_y2 <= 540)
            {
                button2_2.SetTopLeft(420, 490); // 如果滑鼠滑到結束遊戲的按鈕上會有hover的效果
                button2_2.ShowBitmap();
            }
        }

        if (cursor_x2 >= 640 && cursor_x2 <= 695)
        {
            if (cursor_y2 >= 390 && cursor_y2 <= 435)
            {
                about_button_hover.SetTopLeft(640, 390); // 如果滑鼠滑到About icon的按鈕上會有hover的效果
                about_button_hover.ShowBitmap();
            }
        }

        if (cursor_x2 >= 640 && cursor_x2 <= 695)
        {
            if (cursor_y2 >= 320 && cursor_y2 <= 365)
            {
                intro_button_hover.SetTopLeft(640, 320); // 如果滑鼠滑到開始遊戲的按鈕上會有hover的效果
                intro_button_hover.ShowBitmap();
            }
        }

        // 聲音圖案

        if (change == false) // 無靜音
        {
            voice1.SetTopLeft(630, 450);
            voice1.ShowBitmap();

            if (cursor_x2 >= 630 && cursor_x2 <= 690)
            {
                if (cursor_y2 >= 450 && cursor_y2 <= 510)
                {
                    voice2.SetTopLeft(630, 450); // 如果滑鼠滑到音樂控制的按鈕上會有hover的效果
                    voice2.ShowBitmap();
                }
            }
        }

        if (change == true) // 靜音
        {
            voice4.SetTopLeft(630, 450);
            voice4.ShowBitmap();

            if (cursor_x2 >= 630 && cursor_x2 <= 690)
            {
                if (cursor_y2 >= 450 && cursor_y2 <= 510)
                {
                    voice3.SetTopLeft(630, 450); // 如果滑鼠滑到音樂控制的按鈕上會有hover的效果
                    voice3.ShowBitmap();
                }
            }
        }

        if (view == 1) // 當view切到1
        {
            about.SetTopLeft((SIZE_X - about.Width()) / 2, (SIZE_Y - about.Height()) / 8); // 將About畫面的背景及按鈕圖片設置好位置
            about.ShowBitmap();
            before.SetTopLeft(50, 300);
            before.ShowBitmap();

            if (cursor_x2 >= 50 && cursor_x2 <= 95)
            {
                if (cursor_y2 >= 300 && cursor_y2 <= 350)
                {
                    before_hover.SetTopLeft(50, 300); // 如果滑鼠滑到返回的按鈕上會有hover的效果
                    before_hover.ShowBitmap();
                }
            }
        }

        if (view == 2) // 當view切到2
        {
            intro[now].SetTopLeft(0, 0); // 將畫面的背景與按鈕的圖片設置好位置
            intro[now].OnShow();
            backInit.SetTopLeft(300, 518);
            backInit.ShowBitmap();

            if (cursor_x2 >= 35 && cursor_x2 <= 75)
            {
                if (cursor_y2 >= 310 && cursor_y2 <= 370)
                {
                    if (now > 0)
                    {
                        before.SetTopLeft(35, 310); // 如果滑鼠滑到上一頁的按鈕上會有hover的效果
                        before.ShowBitmap();
                    }
                }
            }

            if (cursor_x2 >= 720 && cursor_x2 <= 770)
            {
                if (cursor_y2 >= 310 && cursor_y2 <= 370)
                {
                    if (now < 5)
                    {
                        next.SetTopLeft(720, 310); // 如果滑鼠滑到下一頁的按鈕上會有hover的效果
                        next.ShowBitmap();
                    }
                }
            }

            if (cursor_x2 >= 300 && cursor_x2 <= 495)
            {
                if (cursor_y2 >= 518 && cursor_y2 <= 570)
                {
                    backInit_hover.SetTopLeft(300, 518); // 如果滑鼠滑到回主選單的按鈕上會有hover的效果
                    backInit_hover.ShowBitmap();
                }
            }

            // view == 1 時的聲音圖案

            if (change == false) // 無靜音
            {
                init_voice1.SetTopLeft(580, 505);
                init_voice1.ShowBitmap();

                if (cursor_x2 >= 580 && cursor_x2 <= 640)
                {
                    if (cursor_y2 >= 505 && cursor_y2 <= 570)
                    {
                        init_voice2.SetTopLeft(580, 505); // 如果滑鼠滑到音樂控制的按鈕上會有hover的效果
                        init_voice2.ShowBitmap();
                    }
                }
            }

            if (change == true) // 靜音
            {
                init_voice4.SetTopLeft(580, 505);
                init_voice4.ShowBitmap();

                if (cursor_x2 >= 580 && cursor_x2 <= 640)
                {
                    if (cursor_y2 >= 505 && cursor_y2 <= 570)
                    {
                        init_voice3.SetTopLeft(580, 505); // 如果滑鼠滑到音樂控制的按鈕上會有hover的效果
                        init_voice3.ShowBitmap();
                    }
                }
            }
        }
    }
}