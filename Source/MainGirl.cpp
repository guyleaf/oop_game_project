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
        if (moving) //�ˬd�O�_���b����
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

        if (direction) //���ʤ�V
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
        if (point.x - (map->ScreenX(x) + width) > 0) //�ƹ��y�лP�H���̥k�䪺�y�Ь۴�(�ù����I�y��) �ݤj��0
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