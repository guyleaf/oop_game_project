#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    MainGirl::MainGirl() : x(450), y(220), moving(false), velocity(5), is_focusing(false)
    {
    }

    void MainGirl::LoadBitMap()
    {
        girl_left_stand.LoadBitmap("RES/girl/left/stand.bmp", RGB(0, 0, 0));
        girl_right_stand.LoadBitmap("RES/girl/right/stand.bmp", RGB(0, 0, 0));
        girl_left_focusing_behind.LoadBitmap("RES/girl/left/focusing_behind.bmp", RGB(230, 230, 196));
        girl_left_focusing_front.LoadBitmap("RES/girl/left/focusing_front.bmp", RGB(0, 0, 0));
        girl_right_focusing_behind.LoadBitmap("RES/girl/right/focusing_behind.bmp", RGB(230, 230, 196));
        girl_right_focusing_front.LoadBitmap("RES/girl/right/focusing_front.bmp", RGB(0, 0, 0));

        for (int i = 1; i <= 7; i++)
        {
            char text[100] = {0};
            strcpy(text, ("RES/girl/right/girl_walk (" + to_string(i) + ").bmp").c_str());
            girl_walk_right.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 7; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/left/girl_walk (" + to_string(i) + ").bmp").c_str());
            girl_walk_left.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 7; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/right/girl_run (" + to_string(i) + ").bmp").c_str());
            girl_run_right.AddBitmap(text, RGB(0, 0, 0));
        }

        for (int i = 1; i <= 7; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/left/girl_run (" + to_string(i) + ").bmp").c_str());
            girl_run_left.AddBitmap(text, RGB(0, 0, 0));
        }

        for (int i = 1; i <= 3; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/focus_point_on (" + to_string(i) + ").bmp").c_str());
            focus_point_on.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 4; i++)
        {
            char text[100] = { 0 };
            strcpy(text, ("RES/girl/focus_point_off (" + to_string(i) + ").bmp").c_str());
            focus_point_off.AddBitmap(text, RGB(230, 230, 196));
        }

        focus_point_on.SetDelayCount(1);
        focus_point_off.SetDelayCount(1);
        girl_run_left.SetDelayCount(5);
        girl_run_right.SetDelayCount(5);
    }

    void MainGirl::OnMove(CGameMap* map)
    {
        if (is_focusing)
        {
            focus_point_off.Reset();

            if (!focus_point_on.IsFinalBitmap())
                focus_point_on.OnMove();
        }
        else
        {
            focus_point_on.Reset();

            if (!focus_point_off.IsFinalBitmap())
                focus_point_off.OnMove();
        }

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
            {
                if (velocity != 12)
                    girl_walk_right.OnMove();
                else
                    girl_run_right.OnMove();
            }
            else
            {
                if (velocity != 12)
                    girl_walk_left.OnMove();
                else
                    girl_run_left.OnMove();
            }
        }
    }

    void MainGirl::OnMouseMove(CGameMap* map, CPoint point)
    {
        if (!is_focusing)
        {
            SetMoving(map, point);
            SetVelocity(map, point);
        }
        else
            moving = false;

        cursor_x = point.x;
        cursor_y = point.y;
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
        }
        else if (distance > 150)
        {
            velocity = 8;
            girl_walk_left.SetDelayCount(3);
            girl_walk_right.SetDelayCount(3);
        }
        else
        {
            velocity = 5;
            girl_walk_left.SetDelayCount(5);
            girl_walk_right.SetDelayCount(5);
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
        if (is_focusing)
        {
            if (map->ScreenX(x) + girl_left_stand.Width() / 2 <= cursor_x)
            {
                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y)
                {
                    girl_right_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_front.ShowBitmap();
                }
                else
                {
                    girl_right_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_behind.ShowBitmap();
                }
            }
            else
            {
                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y)
                {
                    girl_left_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_front.ShowBitmap();
                }
                else
                {
                    girl_left_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_behind.ShowBitmap();
                }
            }
        }
        else if (moving) //�O�_���b����
        {
            if (direction) //false => ����, true => ���k
            {
                if (velocity != 12)
                {
                    girl_walk_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_walk_right.OnShow();
                }
                else
                {
                    girl_run_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_run_right.OnShow();
                }
            }
            else
            {
                if (velocity != 12)
                {
                    girl_walk_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_walk_left.OnShow();
                }
                else
                {
                    girl_run_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_run_left.OnShow();
                }
            }
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

    int MainGirl::GetCursorX()
    {
        return cursor_x;
    }

    int MainGirl::GetCursorY()
    {
        return cursor_y;
    }

    void MainGirl::SetIsFocusing(bool status)
    {
        is_focusing = status;
    }

    bool MainGirl::IsFocusing()
    {
        return is_focusing;
    }

    void MainGirl::SetFocusPerson(CGameMap* map, Man* man)
    {
        focus_point_on.SetTopLeft(map->ScreenX(man->GetX()), map->ScreenY(man->GetY()));
        focus_point_off.SetTopLeft(map->ScreenX(man->GetX()), map->ScreenY(man->GetY()));
    }

    void MainGirl::ShowFocus()
    {
        if (is_focusing)
        {
            focus_point_on.OnShow();
        }
        else
        {
            if (!focus_point_off.IsFinalBitmap())
                focus_point_off.OnShow();
        }
    }
}