#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum state
    {
        NORMAL, // 正常模式
        REINFORCING, // 特殊時間過場狀態
        REINFORCED // 特殊時間狀態
    };

    int CHeartPoint::normal = NORMAL;
    int CHeartPoint::reinforcing = REINFORCING;
    int CHeartPoint::reinforced = REINFORCED;

    CHeartPoint::CHeartPoint(int points, int hearts) : NUMPOINTSPERHEART(points), NUMHEARTS(hearts)
    {
        isBmpLoaded = false;
        isWarned = false;
        state = NORMAL;
    }

    void CHeartPoint::Add(int x) // 增加Point值
    {
        if (this->n + x < 0)
            this->n = 0;
        else if (this->n + x < NUMHEARTS * NUMPOINTSPERHEART)
            this->n += x;
        else
            this->n = NUMHEARTS * NUMPOINTSPERHEART;
    }

    int CHeartPoint::GetPoint() // 回傳Point值
    {
        return n;
    }

    void CHeartPoint::LoadBitmap()
    {
        char text[100] = { 0 };

        for (int i = 1; i <= 20; i++)
        {
            strcpy(text, ("RES/UI/heart/heart (" + to_string(i) + ").bmp").c_str());
            hearts[i - 1].LoadBitmap(text, RGB(14, 83, 13));
        }

        for (int i = 1; i <= 23; i++)
        {
            strcpy(text, ("RES/UI/reinforcing/star (" + to_string(i) + ").bmp").c_str());
            star.AddBitmap(text, RGB(0, 0, 0));
        }

        star.SetDelayCount(1);

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/UI/reinforced/reinforced_bar (" + to_string(i) + ").bmp").c_str());
            reinforced_bar.AddBitmap(text, RGB(255, 255, 255));
        }

        reinforced_bar.SetDelayCount(3);

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/UI/heart/warning (" + to_string(i) + ").bmp").c_str());
            warning.AddBitmap(text, RGB(0, 0, 0));
        }

        isBmpLoaded = true;
    }

    void CHeartPoint::SetPoint(int n) // 設定Point值
    {
        this->n = n;
    }

    void CHeartPoint::SetTopLeft(int x, int y) // 將動畫的左上角座標移至 (x,y)
    {
        this->x = x;
        this->y = y;
    }

    void CHeartPoint::OnMove(UI* ui)
    {
        if (state == REINFORCED) // 特殊時間狀態
        {
            if (!reinforced_bar.IsFinalBitmap())
                reinforced_bar.OnMove();
            else
                star.OnMove();
        }
        else
        {
            star.Reset();
            reinforced_bar.Reset();

            if (!ui->IsGameOver() && n <= 1000) // 小於1000時，發出警告
            {
                if (!isWarned)
                {
                    CAudio::Instance()->Play(AUDIO_WARNING, false);
                    isWarned = true;
                }

                warning.OnMove();
            }
            else
                isWarned = false;
        }
    }

    void CHeartPoint::OnShow(UI* ui, int counter)
    {
        GAME_ASSERT(isBmpLoaded, "CHeartPoint: 請先執行LoadBitmap，然後才能ShowBitmap");
        static int index = 0;
        int nx;		// 待顯示位數的 x 座標
        int MSB;	// 最左邊(含符號)的位數的數值
        MSB = n;
        nx = x;

        if (state == NORMAL) // 正常模式
        {
            for (int i = 0; i < NUMHEARTS; i++)
            {
                if (MSB < 0)
                    MSB = 0;

                if (MSB - NUMPOINTSPERHEART >= 0)
                {
                    hearts[19].SetTopLeft(nx, y);
                    hearts[19].ShowBitmap();
                }
                else
                {
                    int d = MSB / 25;
                    hearts[d].SetTopLeft(nx, y);
                    hearts[d].ShowBitmap();
                }

                if (!ui->IsGameOver() && n <= 1000)
                {
                    warning.SetTopLeft(nx - 6, y - 7);
                    warning.OnShow();
                }

                nx += hearts[19].Width();
                MSB -= NUMPOINTSPERHEART;
            }
        }
        else if (state == REINFORCING) // 特殊時間過場狀態
        {
            index = (index + 1) % NUMHEARTS;

            for (int i = 0; i < NUMHEARTS; i++)
            {
                if (index == i)
                {
                    hearts[0].SetTopLeft(nx, y);
                    hearts[0].ShowBitmap();
                    nx += hearts[0].Width() - 2;
                }
                else
                {
                    hearts[19].SetTopLeft(nx, y);
                    hearts[19].ShowBitmap();
                    nx += hearts[19].Width() - 2;
                }
            }
        }
        else if (state == REINFORCED) // 特殊時間狀態
        {
            reinforced_bar.SetTopLeft(x, y - 10);
            reinforced_bar.OnShow();

            if (reinforced_bar.IsFinalBitmap()) // 控制量條
            {
                CDC* pDC = CDDraw::GetBackCDC();
                CPen ppen(PS_SOLID, 0, RGB(253, 165, 253));
                CBrush pbrush(RGB(253, 165, 253));
                CPen* pOldPen = pDC->SelectObject(&ppen);
                CBrush* pOldBrush = pDC->SelectObject(&pbrush);
                CRect rect;
                rect.SetRect(x + 54, y + 5, int(x + 54 + n / 27.10), y + reinforced_bar.Height() - 20);
                pDC->RoundRect(rect, CPoint(0, 0));
                pDC->SelectObject(pOldPen);
                pDC->SelectObject(pOldBrush);
                CDDraw::ReleaseBackCDC();
                ////////////////////////////////////////////////////
                star.SetTopLeft(x + 55, y + 5);
                star.OnShow();
                ////////////////////////////////////////////////////
                pDC = CDDraw::GetBackCDC();
                CPen gpen(PS_SOLID, 0, RGB(15, 84, 14));
                CBrush gbrush(RGB(15, 84, 14));
                pOldPen = pDC->SelectObject(&gpen);
                pOldBrush = pDC->SelectObject(&gbrush);
                rect.SetRect(int(x + 54 + n / 27.10), y + 5, x + reinforced_bar.Width() - 54, y + reinforced_bar.Height() - 20);
                pDC->RoundRect(rect, CPoint(0, 0));
                pDC->SelectObject(pOldPen);
                pDC->SelectObject(pOldBrush);
                CDDraw::ReleaseBackCDC();
            }
        }
    }

    void CHeartPoint::GotoHRState(int state)
    {
        this->state = state;
    }
}