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
    MainGirl::MainGirl() : x(100), y(140), moving(false)
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
                if (MAP_W - x > 20)
                    x += 10;

                map->Addsx(10);
            }
            else
            {
                if (x > 10)
                    x -= 10;

                map->Addsx(-10);
            }
        }
    }

    void MainGirl::SetMoving(CGameMap* map, CPoint point)
    {
        if (point.x - map->ScreenX(x) > 10)
        {
            moving = true;
            direction = true;
        }
        else if (map->ScreenX(x) - point.x > 10)
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