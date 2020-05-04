#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    CHeartPoint::CHeartPoint(int hearts) : NUMHEARTS(hearts)
    {
        isBmpLoaded = false;
    }

    void CHeartPoint::Add(int x)
    {
        this->n += x;
    }

    int CHeartPoint::GetPoint()
    {
        return n;
    }

    void CHeartPoint::LoadBitmap()
    {
        char text[100] = { 0 };

        for (int i = 1; i <= 19; i++)
        {
            strcpy(text, ("RES/UI/heart (" + to_string(i) + ").bmp").c_str());
            hearts[i].LoadBitmap(text, RGB(14, 83, 13));
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
    }
}
