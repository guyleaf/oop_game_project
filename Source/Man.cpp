#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum State
    {
        ALIVE,
        DEAD,
        FOLLOW
    };

    Man::Man(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(3)
    {
        range[0] = start;
        range[1] = end;
        status = ALIVE;
        is_focused = false;
        is_attacked = false;
        id = rand();
        fdirection = false;
    }

    void Man::OnMove()
    {
        if (status == ALIVE)
        {
            if (is_attacked)
            {
                flash.OnMove();
                weakening.OnMove();

                if (HP >= 900 && blood.GetCurrentBitmapNumber() != 1)
                    blood.OnMove();
                else if (50 < HP && HP < 900 && blood.GetCurrentBitmapNumber() != ((1000 - HP) / 100 + 1)) //鎖定顯示最終圖片
                    blood.OnMove();
                else if (HP < 50 && !blood.IsFinalBitmap())
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
        else if (status == DEAD)
        {
            if (direction)
            {
                if (!man_dead_right.IsFinalBitmap())
                {
                    man_dead_right.OnMove();
                }
                else
                    status = FOLLOW;
            }
            else
            {
                if (!man_dead_left.IsFinalBitmap())
                {
                    man_dead_left.OnMove();
                }
                else
                    status = FOLLOW;
            }
        }
        else
        {
            if (fx - x >= 8)
            {
                direction = true;
                x += velocity;
            }
            else if (x - fx >= 8)
            {
                direction = false;
                x -= velocity;
            }
            else
            {
                direction = fdirection;
                x = fx;
            }

            if (fy - y >= 8)
            {
                y += velocity;
            }
            else if (y - fy >= 8)
            {
                y -= velocity;
            }
            else
                y = fy;

            if (direction)
                man_following_right.OnMove();
            else
                man_following_left.OnMove();
        }
    }

    void Man::OnShow(CGameMap* map)
    {
        if (status == ALIVE)
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
        else if (status == DEAD)
        {
            if (direction)
            {
                man_dead_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_dead_right.OnShow();
            }
            else
            {
                man_dead_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_dead_left.OnShow();
            }
        }
        else
        {
            if (direction)
            {
                man_following_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_following_right.OnShow();
            }
            else
            {
                man_following_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_following_left.OnShow();
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

    void Man::SetVelocity(int speed)
    {
        velocity = speed;
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
        if (status)
            this->status = ALIVE;
        else
            this->status = DEAD;
    }

    bool Man::IsAlive()
    {
        return this->status == ALIVE;
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

    void Man::Follow(int x, int y, bool direction)
    {
        fx = x;
        fy = y;
        fdirection = direction;
    }

    int Man::GetId()
    {
        return id;
    }

    int Man::GetHP()
    {
        return HP;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    NormalMan::NormalMan(int x, int y, int start, int end, bool direction, int type) : Man(x, y, start, end, direction), type(type)
    {
        HP = 1000;
    }

    void NormalMan::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/alive/left/stand.bmp").c_str());
        man_left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/alive/right/stand.bmp").c_str());
        man_right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/alive/right/normalMan (" + to_string(i) + ").bmp").c_str());
            man_right.AddBitmap(text, RGB(255, 255, 255));
        }

        man_right.SetDelayCount(13);

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/alive/left/normalMan (" + to_string(i) + ").bmp").c_str());
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

        if (type == 1)
        {
            for (int i = 1; i <= 5; i++)
            {
                strcpy(text, ("RES/Man/normalMan1/dead/left/slave (" + to_string(i) + ").bmp").c_str());
                man_dead_left.AddBitmap(text, RGB(255, 255, 255));
            }

            for (int i = 1; i <= 5; i++)
            {
                strcpy(text, ("RES/Man/normalMan1/dead/right/slave (" + to_string(i) + ").bmp").c_str());
                man_dead_right.AddBitmap(text, RGB(255, 255, 255));
            }
        }
        else
        {
            for (int i = 1; i <= 7; i++)
            {
                strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/dead/left/slave (" + to_string(i) + ").bmp").c_str());
                man_dead_left.AddBitmap(text, RGB(255, 255, 255));
            }

            for (int i = 1; i <= 7; i++)
            {
                strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/dead/right/slave (" + to_string(i) + ").bmp").c_str());
                man_dead_right.AddBitmap(text, RGB(255, 255, 255));
            }
        }

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/follow/left/slave (" + to_string(i) + ").bmp").c_str());
            man_following_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/follow/right/slave (" + to_string(i) + ").bmp").c_str());
            man_following_right.AddBitmap(text, RGB(255, 255, 255));
        }

        man_dead_left.SetDelayCount(8);
        man_dead_right.SetDelayCount(8);
        man_following_left.SetDelayCount(8);
        man_following_right.SetDelayCount(8);
    }

}
