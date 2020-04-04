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

    void Heart::LoadBitmap()
    {
        if (size == 0)
            heart.LoadBitmap("RES/heart_small.bmp", RGB(255, 255, 255));
        else
            heart.LoadBitmap("RES/heart_big.bmp", RGB(255, 255, 255));
    }

    void Heart::OnShow(CGameMap* map)
    {
        heart.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
        heart.ShowBitmap();
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
