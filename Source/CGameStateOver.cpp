#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateOver.h"
#include <string>

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的結束狀態(Game Over)
    /////////////////////////////////////////////////////////////////////////////

    CGameStateOver::CGameStateOver(CGame* g, int* score)
        : CGameState(g, score)
    {
    }

    void CGameStateOver::OnMove()
    {
        if (playAgain)
        {
            counter--;

            if (counter <= 0)
                GotoGameState(GAME_STATE_INIT);
        }

        if (starts[section].IsFinalBitmap())
            loop = true;

        if (!loop)
            starts[section].OnMove();
        else
            loops[section].OnMove();
    }

    void CGameStateOver::OnBeginState()
    {
        waveOutGetVolume(0, &volume);

        if (volume == 0)
        {
            volume = 0xFFFFFFFF;
            change = true;
        }
        else
        {
            change = false;
        }

        loop = false;

        if (*score < 3000)
            section = 0;
        else if (*score < 30000)
            section = 1;
        else if (*score < 70000)
            section = 2;
        else if (*score < 100000)
            section = 3;
        else if (*score < 150000)
            section = 4;
        else
            section = 5;

        for (int i = 0; i < 4; i++)
        {
            starts[i].Reset();
            loops[i].Reset();
        }

        if (section == 0)
            CAudio::Instance()->Play(AUDIO_END1, true);
        else if (section == 1)
            CAudio::Instance()->Play(AUDIO_END2, true);
        else if (section == 2)
            CAudio::Instance()->Play(AUDIO_END3, true);
        else if (section == 3)
            CAudio::Instance()->Play(AUDIO_END4, true);
        else
            CAudio::Instance()->Play(AUDIO_END5, true);

        cursor_x = cursor_y = -1;
        playAgain = false;
        counter = 15;
    }

    void CGameStateOver::OnInit()
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
        //
        // 開始載入資料
        //
        char text[100] = { 0 };

        for (int i = 1; i <= 10; i++)
        {
            strcpy(text, ("RES/end/end1/start (" + to_string(i) + ").bmp").c_str());
            starts[0].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/end/end1/loop (" + to_string(i) + ").bmp").c_str());
            loops[0].AddBitmap(text);
        }

        for (int i = 1; i <= 6; i++)
        {
            strcpy(text, ("RES/end/end2/start (" + to_string(i) + ").bmp").c_str());
            starts[1].AddBitmap(text);
        }

        for (int i = 1; i <= 8; i++)
        {
            strcpy(text, ("RES/end/end2/loop (" + to_string(i) + ").bmp").c_str());
            loops[1].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/end/end3/start (" + to_string(i) + ").bmp").c_str());
            starts[2].AddBitmap(text);
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/end/end3/loop (" + to_string(i) + ").bmp").c_str());
            loops[2].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/end/end4/start (" + to_string(i) + ").bmp").c_str());
            starts[3].AddBitmap(text);
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/end/end4/loop (" + to_string(i) + ").bmp").c_str());
            loops[3].AddBitmap(text);
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/end/end5/start (" + to_string(i) + ").bmp").c_str());
            starts[4].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/end/end5/loop (" + to_string(i) + ").bmp").c_str());
            loops[4].AddBitmap(text);
        }

        for (int i = 1; i <= 1; i++)
        {
            strcpy(text, ("RES/end/end6/start (" + to_string(i) + ").bmp").c_str());
            starts[5].AddBitmap(text);
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/end/end6/loop (" + to_string(i) + ").bmp").c_str());
            loops[5].AddBitmap(text);
        }

        for (int i = 0; i < 6; i++)
        {
            starts[i].SetDelayCount(9);
            loops[i].SetDelayCount(9);
        }

        voice_button_on.LoadBitmap("RES/end/voice_button_on.bmp", RGB(255, 255, 255));
        voice_button_on_hover.LoadBitmap("RES/end/voice_button_on_hover.bmp", RGB(255, 255, 255));
        voice_button_off.LoadBitmap("RES/end/voice_button_off.bmp", RGB(255, 255, 255));
        voice_button_off_hover.LoadBitmap("RES/end/voice_button_off_hover.bmp", RGB(255, 255, 255));
        restart.LoadBitmap("RES/end/restart.bmp", RGB(255, 255, 255));
        restart_hover.LoadBitmap("RES/end/restart_hover.bmp", RGB(255, 255, 255));
        exit.LoadBitmap("RES/end/exit.bmp", RGB(255, 255, 255));
        exit_hover.LoadBitmap("RES/end/exit_hover.bmp", RGB(255, 255, 255));

        for (int i = 0; i < 10; i++)
        {
            strcpy(text, ("RES/end/" + to_string(i) + ".bmp").c_str());
            num[i].LoadBitmap(text, RGB(255, 255, 255));
        }

        ShowInitProgress(87);
        CAudio::Instance()->Load(AUDIO_END1, "Sounds/end1.mp3");
        CAudio::Instance()->Load(AUDIO_END2, "Sounds/end2.mp3");
        CAudio::Instance()->Load(AUDIO_END3, "Sounds/end3.mp3");
        CAudio::Instance()->Load(AUDIO_END4, "Sounds/end4.mp3");
        CAudio::Instance()->Load(AUDIO_END5, "Sounds/end5.mp3");
        //Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //
        // 最終進度為100%
        //
        ShowInitProgress(100);
    }

    void CGameStateOver::OnMouseMove(UINT nFlags, CPoint point)
    {
        cursor_x = point.x;
        cursor_y = point.y;
    }

    void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
    {
    }

    void CGameStateOver::OnLButtonUp(UINT nFlags, CPoint point)
    {
        if (155 <= cursor_x && cursor_x <= 350 && 520 <= cursor_y && cursor_y <= 580)
        {
            playAgain = true;
            CAudio::Instance()->Stop(AUDIO_END1);
            CAudio::Instance()->Stop(AUDIO_END2);
            CAudio::Instance()->Stop(AUDIO_END3);
            CAudio::Instance()->Stop(AUDIO_END4);
            CAudio::Instance()->Stop(AUDIO_END5);
            CAudio::Instance()->Play(AUDIO_PRESS);
        }
        else if (380 <= cursor_x && cursor_x <= 575 && 523 <= cursor_y && cursor_y <= 583)
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
        else if (610 <= cursor_x && cursor_x <= 670 && 510 <= cursor_y && cursor_y <= 590)
        {
            if (!change)
                waveOutSetVolume(0, 0); //On切至Off時靜音
            else
                waveOutSetVolume(0, volume); //Off切至On時取消靜音

            change = !change;
        }
    }

    void CGameStateOver::OnShow()
    {
        if (!loop)
        {
            starts[section].SetTopLeft(0, 0);
            starts[section].OnShow();
        }
        else
        {
            loops[section].SetTopLeft(0, 0);
            loops[section].OnShow();
        }

        int tmp = *score;
        int nx = SIZE_X / 2 - 117;
        int ny = 32;

        for (int i = 7; i >= 0; i--)
        {
            int n = tmp / (int)(pow(10, i));
            tmp %= (int)(pow(10, i));

            if (i >= 5)
            {
                num[n].SetTopLeft(nx, ny);
                num[n].ShowBitmap();
                ny += (15 - (int)(pow(7 - i, 2)) * 3);
            }
            else if (i <= 2)
            {
                ny -= (15 - (int)(pow(i, 2)) * 3);
                num[n].SetTopLeft(nx, ny);
                num[n].ShowBitmap();
            }
            else
            {
                num[n].SetTopLeft(nx, ny);
                num[n].ShowBitmap();
            }

            nx += (num[n].Width() - 1);
        }

        restart.SetTopLeft(155, 520);
        restart.ShowBitmap();
        exit.SetTopLeft(380, 518);
        exit.ShowBitmap();

        if (!change)
        {
            voice_button_on.SetTopLeft(600, 510);
            voice_button_on.ShowBitmap();
        }
        else
        {
            voice_button_off.SetTopLeft(600, 510);
            voice_button_off.ShowBitmap();
        }

        if (155 <= cursor_x && cursor_x <= 350 && 520 <= cursor_y && cursor_y <= 580)
        {
            restart_hover.SetTopLeft(155, 520);
            restart_hover.ShowBitmap();
        }
        else if (380 <= cursor_x && cursor_x <= 575 && 523 <= cursor_y && cursor_y <= 583)
        {
            exit_hover.SetTopLeft(383, 521);
            exit_hover.ShowBitmap();
        }
        else if (600 <= cursor_x && cursor_x <= 660 && 510 <= cursor_y && cursor_y <= 590)
        {
            if (!change)
            {
                voice_button_on_hover.SetTopLeft(600, 510);
                voice_button_on_hover.ShowBitmap();
            }
            else
            {
                voice_button_off_hover.SetTopLeft(600, 510);
                voice_button_off_hover.ShowBitmap();
            }
        }

        if (playAgain)
        {
            CDC* pDC = CDDraw::GetBackCDC();
            CDC ImageDC;
            ImageDC.CreateCompatibleDC(pDC);
            HBITMAP tmp = CreateCompatibleBitmap(ImageDC.m_hDC, SIZE_X, SIZE_Y);
            HBITMAP* pOldBitmap = (HBITMAP*)ImageDC.SelectObject(tmp);
            ImageDC.SetBkColor(RGB(0, 0, 0));
            BLENDFUNCTION bf;
            bf.AlphaFormat = 0;
            bf.BlendFlags = 0;
            bf.BlendOp = 0;
            bf.SourceConstantAlpha = 17 * (15 - counter);
            pDC->AlphaBlend(0, 0, SIZE_X, SIZE_Y, &ImageDC, 0, 0, SIZE_X, SIZE_Y, bf);
            ImageDC.SelectObject(pOldBitmap);
            ImageDC.DeleteDC();
            CDDraw::ReleaseBackCDC();
        }
    }
}