#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "Girl.h"

namespace game_framework
{
    Girl::Girl(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(false), direction(direction), velocity(5)
    {
        range[0] = start;
        range[1] = end;
    }

    void Girl::LoadBitmap()
    {
    }

    void Girl::SetDirection(bool direction)
    {
        this->direction = direction;
    }

    void Girl::SetMoving(bool status)
    {
        moving - status;
    }

    void Girl::OnMove()
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
                girl_right.OnMove();
            else
                girl_right.OnMove();
        }
    }

    void Girl::OnShow(CGameMap* map)
    {
        if (moving) //是否正在移動
            if (direction) //false => 往左, true => 往右
            {
                girl_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                girl_right.OnShow();
            }
            else
            {
                girl_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                girl_left.OnShow();
            }
        else
        {
            if (direction) //false => 往左, true => 往右
            {
                girl_right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                girl_right_stand.ShowBitmap();
            }
            else
            {
                girl_left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                girl_left_stand.ShowBitmap();
            }
        }
    }
}