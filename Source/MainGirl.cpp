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
    MainGirl::MainGirl() : x(450), y(220), moving(false), velocity(5)
    {
    }

    void MainGirl::LoadBitMap()
    {
        girl_left_stand.LoadBitmap("RES/girl/left/stand.bmp", RGB(255, 255, 255));
        girl_right_stand.LoadBitmap("RES/girl/right/stand.bmp", RGB(255, 255, 255));

        for (int i = 1; i <= 16; i++)
        {
            char text[100] = {0};
            strcpy(text, ("RES/girl/right/girl (" + to_string(i) + ").bmp").c_str());
            girl_right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 16; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/left/girl (" + to_string(i) + ").bmp").c_str());
            girl_left.AddBitmap(text, RGB(255, 255, 255));
        }
    }

    void MainGirl::OnMove(CGameMap* map)
    {
        if (moving) //�ˬd�O�_���b����
        {
            if (direction)
            {
                if (MAP_W - (x + girl_right_stand.Width()) > 10)
                    x += velocity;

                map->Addsx(velocity);
            }
            else
            {
                if (x > 10)
                    x -= velocity;

                map->Addsx(-velocity);
            }

            if (direction)
                girl_right.OnMove();
            else
                girl_left.OnMove();
        }
    }

    void MainGirl::OnMouseMove(CGameMap* map, CPoint point)
    {
        SetMoving(map, point);
        SetVelocity(map, point);
    }

    void MainGirl::SetVelocity(CGameMap* map, CPoint point)
    {
        if (!moving) //�S�����b���ʫh�h�X
            return;

        int distance; //�ƹ����ù��I�y�Ш�H�����Z��

        if (direction) //false => ����, true => ���k
            distance = point.x - (map->ScreenX(x) + girl_right_stand.Width());
        else
            distance = map->ScreenX(x) - point.x;

        if (distance > 300) //�Z���V���t�׶V��
        {
            velocity = 12;
            girl_left.SetDelayCount(1);
            girl_right.SetDelayCount(1);
        }
        else if (distance > 150)
        {
            velocity = 8;
            girl_left.SetDelayCount(3);
            girl_right.SetDelayCount(3);
        }
        else
        {
            velocity = 5;
            girl_left.SetDelayCount(5);
            girl_right.SetDelayCount(5);
        }
    }

    void MainGirl::SetMoving(CGameMap* map, CPoint point)
    {
        if (point.x - (map->ScreenX(x) + girl_right_stand.Width()) > 0) //�ƹ��y�лP�H���̥k�䪺�y�Ь۴�(�ù����I�y��) �ݤj��0
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
        if (moving) //�O�_���b����
            if (direction) //false => ����, true => ���k
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
            if (direction) //false => ����, true => ���k
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