#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    CHeartPoint::CHeartPoint(int points, int hearts) : NUMPOINTSPERHEART(points), NUMHEARTS(hearts)
    {
        isBmpLoaded = false;
    }

    void CHeartPoint::Add(int x)
    {
        if (this->n + x < NUMHEARTS * NUMPOINTSPERHEART)
            this->n += x;
        else
            this->n = NUMHEARTS * NUMPOINTSPERHEART;
    }

    int CHeartPoint::GetPoint()
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

        isBmpLoaded = true;
    }

    void CHeartPoint::SetPoint(int n)
    {
        this->n = n;
    }

    void CHeartPoint::SetTopLeft(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void CHeartPoint::ShowBitmap()
    {
        GAME_ASSERT(isBmpLoaded, "CHeartPoint: 請先執行LoadBitmap，然後才能ShowBitmap");
        int nx;		// 待顯示位數的 x 座標
        int MSB;	// 最左邊(含符號)的位數的數值
        MSB = n;
        nx = x;

        for (int i = 0; i < NUMHEARTS; i++)
        {
            if (MSB < 0)
                MSB = 0;

            if (MSB - NUMPOINTSPERHEART >= 0)
            {
                hearts[19].SetTopLeft(nx, y);
                hearts[19].ShowBitmap();
                nx += hearts[19].Width() - 2;
            }
            else
            {
                int d = MSB / 25;
                hearts[d].SetTopLeft(nx, y);
                hearts[d].ShowBitmap();
                nx += hearts[d].Width() - 2;
            }

            MSB -= NUMPOINTSPERHEART;
        }
    }
}
