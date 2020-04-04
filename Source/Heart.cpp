#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    Heart::Heart(int size, int x, int y, int HP) : size(size), x(x), y(y), HP(HP)
    {
        LoadBitmap();
    }

    void Heart::OnMove()
    {
        if (!heart.IsFinalBitmap())
            heart.OnMove();
    }

    void Heart::LoadBitmap()
    {
        char text[100] = { 0 };

        for (int i = 1; i <= 15; i++)
        {
            if (size == 0)
            {
                strcpy(text, ("RES/Heart/heart_small (" + to_string(i) + ").bmp").c_str());
                heart.AddBitmap(text, RGB(0, 0, 0));
            }
            else
            {
                strcpy(text, ("RES/Heart/heart_big (" + to_string(i) + ").bmp").c_str());
                heart.AddBitmap(text, RGB(0, 0, 0));
            }
        }

        heart.SetDelayCount(1);
    }

    void Heart::OnShow(CGameMap* map)
    {
        heart.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
        heart.OnShow();
    }

    bool Heart::HitMainGirl(MainGirl* girl)
    {
        return girl->GetPositionX() <= this->x && this->x <= girl->GetPositionX() + girl->Width();
    }

    int Heart::GetHP()
    {
        return HP;
    }
}
