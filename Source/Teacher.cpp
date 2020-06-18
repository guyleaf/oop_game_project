#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    Teacher::Teacher(int level, int x, int y)
    {
        this->x = x;
        this->y = y;
        this->level = level;
        direction = false;
    }

    void Teacher::LoadBitmap()
    {
        char text[100] = { 0 };

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/teacher/left/left (" + to_string(i) + ").bmp").c_str());
            left.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/teacher/right/right (" + to_string(i) + ").bmp").c_str());
            right.AddBitmap(text, RGB(230, 230, 196));
        }

        left.SetDelayCount(5);
        right.SetDelayCount(5);
    }

    void Teacher::OnMove(CGameMap* map)
    {
        if (!map->IsEmpty(x, y)) // 只在地圖邊界範圍內移動
            direction = !direction;

        if (direction)
        {
            x += 5;
            right.OnMove();
        }
        else
        {
            x -= 5;
            left.OnMove();
        }
    }

    void Teacher::OnShow(CGameMap* map)
    {
        if (map->GetLevel() == level)
        {
            if (direction)
            {
                right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                right.OnShow();
            }
            else
            {
                left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                left.OnShow();
            }
        }
    }

    bool Teacher::HitMainGirl(MainGirl* girl) // 判斷是否與女主角接觸
    {
        return girl->GetPositionX() <= x && x <= girl->GetPositionX() + girl->Width();
    }
    bool Teacher::IsInLevel(int level) // 老師是否在level層
    {
        return this->level == level;
    }
}

