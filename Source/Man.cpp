#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "Man.h"

namespace game_framework
{
    Man::Man(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(3)
    {
        range[0] = start;
        range[1] = end;
    }

    void Man::OnMove()
    {
        if (moving)
        {
            if (direction)
            {
                if (x < range[1])
                    x += velocity;
                else
                    SetDirection(false);
            }
            else
            {
                if (x > range[0])
                    x -= velocity;
                else
                    SetDirection(true);
            }

            if (direction)
            {
                man_right.OnMove();
            }
            else
            {
                man_left.OnMove();
            }
        }
    }

    void Man::OnShow(CGameMap* map)
    {
        if (moving) //是否正在移動
            if (direction) //false => 往左, true => 往右
            {
                man_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_right.OnShow();
            }
            else
            {
                man_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_left.OnShow();
            }
        else
        {
            if (direction) //false => 往左, true => 往右
            {
                man_right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_right_stand.ShowBitmap();
            }
            else
            {
                man_left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_left_stand.ShowBitmap();
            }
        }
    }


    void Man::SetDirection(bool direction)
    {
        this->direction = direction;
    }

    void Man::SetMoving(bool status)
    {
        moving = status;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    NormalMan::NormalMan(int x, int y, int start, int end, bool direction, int type) : Man(x, y, start, end, direction), type(type)
    {
    }

    void NormalMan::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, ("RES/normalMan" + to_string(type) + "/left/stand.bmp").c_str());
        man_left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/normalMan" + to_string(type) + "/right/stand.bmp").c_str());
        man_right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/normalMan" + to_string(type) + "/right/normalMan (" + to_string(i) + ").bmp").c_str());
            man_right.AddBitmap(text, RGB(255, 255, 255));
        }

        man_right.SetDelayCount(13);

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/normalMan" + to_string(type) + "/left/normalMan (" + to_string(i) + ").bmp").c_str());
            man_left.AddBitmap(text, RGB(255, 255, 255));
        }

        man_left.SetDelayCount(13);
    }
}
