#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "MainGirl.h"

namespace game_framework
{
    MainGirl::MainGirl() : x(450), y(140), moving(false), velocity(5), width(150), height(194)
    {
    }

    void MainGirl::LoadBitMap()
    {
        girl.LoadBitmap(IDB_GIRLLEFT, RGB(0, 0, 0));
    }

    void MainGirl::OnMove(CGameMap* map)
    {
        if (moving) //檢查是否正在移動
        {
            if (direction)
            {
                if (MAP_W - (x + width) > 10)
                    x += velocity;

                map->Addsx(velocity);
            }
            else
            {
                if (x > 10)
                    x -= velocity;

                map->Addsx(-velocity);
            }
        }
    }

    void MainGirl::SetVelocity(CGameMap* map, CPoint point)
    {
        if (!moving)
            return;

        int distance;

        if (direction) //移動方向
            distance = point.x - (map->ScreenX(x) + width);
        else
            distance = map->ScreenX(x) - point.x;

        if (distance > 300)
            velocity = 12;
        else if (distance > 150)
            velocity = 8;
        else
            velocity = 5;
    }

    void MainGirl::SetMoving(CGameMap* map, CPoint point)
    {
        if (point.x - (map->ScreenX(x) + width) > 0) //滑鼠座標與人物最右邊的座標相減(螢幕的點座標) 需大於0
        {
            moving = true;
            direction = true;
        }
        else if (map->ScreenX(x) - point.x > 0)
        {
            moving = true;
            direction = false;
        }
        else
            moving = false;
    }

    void MainGirl::OnShow(CGameMap* map)
    {
        girl.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
        girl.ShowBitmap();
    }
}