#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    Man::Man(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(3)
    {
        range[0] = start;
        range[1] = end;
        is_alive = true;
        is_focused = false;
        is_attacked = false;
        id = rand();
    }

    void Man::OnMove()
    {
        if (is_attacked)
        {
            flash.OnMove();
            weakening.OnMove();

            if (HP >= 900 && blood.GetCurrentBitmapNumber() != 1)
                blood.OnMove();
            else if (0 < HP && HP < 900 && blood.GetCurrentBitmapNumber() != ((1000 - HP) / 100 + 1)) //鎖定顯示最終圖片
                blood.OnMove();
            else if (HP == 0 && !blood.IsFinalBitmap())
                blood.OnMove();

            is_attacked = false;
            return;
        }
        else if (moving)
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
        if (is_attacked)
        {
            flash.SetTopLeft(map->ScreenX(x) - 18, map->ScreenY(y));
            flash.OnShow();
            weakening.SetTopLeft(map->ScreenX(x), map->ScreenY(y) + 5);
            weakening.OnShow();
            blood.SetTopLeft(map->ScreenX(x), map->ScreenY(y) - blood.Height());
            blood.OnShow();
        }
        else if (moving) //是否正在移動
        {
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

    bool Man::HitMainGirl(CGameMap* map, MainGirl* girl)
    {
        int cursor_x = girl->GetCursorX();
        int cursor_y = girl->GetCursorY();
        int x2 = x + man_left_stand.Width();
        int y2 = y + man_left_stand.Height();
        return (cursor_x >= map->ScreenX(x) && cursor_y >= map->ScreenY(y)
                && cursor_x <= map->ScreenX(x2) && cursor_y <= (map->ScreenY(y2) - man_left_stand.Height() / 2));
    }


    void Man::SetIsAlive(bool status)
    {
        is_alive = status;
    }

    bool Man::IsAlive()
    {
        return is_alive;
    }

    void Man::SetIsFocused(bool status)
    {
        is_focused = status;
    }

    bool Man::IsFocused()
    {
        return is_focused;
    }

    int Man::GetX()
    {
        return x;
    }

    int Man::GetY()
    {
        return y;
    }

    int Man::GetWidth()
    {
        return man_left_stand.Width();
    }

    int Man::GetHeight()
    {
        return man_left_stand.Height();
    }

    void Man::LoseHP(int value)
    {
        if (HP > 0)
            HP -= value;
        else
            HP = 0;

        is_attacked = true;
    }

    int Man::GetId()
    {
        return id;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    NormalMan::NormalMan(int x, int y, int start, int end, bool direction, int type) : Man(x, y, start, end, direction), type(type)
    {
        HP = 1000;
    }

    void NormalMan::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/left/stand.bmp").c_str());
        man_left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/right/stand.bmp").c_str());
        man_right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/right/normalMan (" + to_string(i) + ").bmp").c_str());
            man_right.AddBitmap(text, RGB(255, 255, 255));
        }

        man_right.SetDelayCount(13);

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/left/normalMan (" + to_string(i) + ").bmp").c_str());
            man_left.AddBitmap(text, RGB(255, 255, 255));
        }

        man_left.SetDelayCount(13);

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/Man/flash (" + to_string(i) + ").bmp").c_str());
            flash.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 2; i++)
        {
            strcpy(text, ("RES/Man/weakening (" + to_string(i) + ").bmp").c_str());
            weakening.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 14; i++)
        {
            strcpy(text, ("RES/Man/blood (" + to_string(i) + ").bmp").c_str());
            blood.AddBitmap(text, RGB(255, 255, 255));
        }

        blood.SetDelayCount(1);
    }

}
