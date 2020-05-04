#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum Role
    {
        MAINGIRL,
        GIRL,
        ALL
    };

    enum State
    {
        ALIVE,
        ATTACKED_BY_MAINGIRL,
        ATTACKED_BY_ALL,
        DEAD,
        FOLLOW,
        LEAVING
    };

    int Man::mainGirl = MAINGIRL;
    int Man::all = ALL;
    int Man::girl = GIRL;

    CAnimation Man::clicking;
    CMovingBitmap Man::clicking_bar;
    bool Man::bitmapIsLoaded = false;

    Man::Man(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(3)
    {
        range[0] = start;
        range[1] = end;
        status = ALIVE;
        is_focused = false;
        id = rand();
        fdirection = false;
        is_positioned = false;
        distance = 300;
    }

    void Man::OnMove()
    {
        if (status == ALIVE)
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
        else if (status == ATTACKED_BY_MAINGIRL)
        {
            flash.OnMove();
            weakening.OnMove();

            if (HP >= 700 && blood.GetCurrentBitmapNumber() != 1)
                blood.OnMove();
            else if (50 < HP && HP < 700 && blood.GetCurrentBitmapNumber() != ((800 - int(HP)) / 75 + 1)) //鎖定顯示最終圖片
                blood.OnMove();
            else if (HP < 50 && !blood.IsFinalBitmap())
                blood.OnMove();

            status = ALIVE;
        }
        else if (status == ATTACKED_BY_ALL)
        {
            flash_multi.OnMove();
            weakening.OnMove();
            clicking.OnMove();
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
                {
                    if (is_killed_by == MAINGIRL)
                        status = FOLLOW;
                    else
                        status = LEAVING;
                }
            }
            else
            {
                if (!man_dead_left.IsFinalBitmap())
                {
                    man_dead_left.OnMove();
                }
                else
                {
                    if (is_killed_by == MAINGIRL)
                        status = FOLLOW;
                    else
                        status = LEAVING;
                }
            }
        }
        else if (status == FOLLOW)
        {
            //velocity = 20;

            //待修正 after demo
            if (is_positioned && direction != fdirection)
            {
                if (fdirection)
                    x = fx + 80;
                else
                    x = fx - 80;

                direction = fdirection;
                is_positioned = false;
            }
            else if (!is_positioned && fx - x >= 10)
            {
                direction = fdirection;
                x += velocity;
            }
            else if (!is_positioned && x - fx >= 10)
            {
                direction = fdirection;
                x -= velocity;
            }
            else
            {
                is_positioned = true;
                direction = fdirection;
                x = fx;
            }

            if (fy - y >= 20)
            {
                y += velocity;
            }
            else if (y - fy >= 20)
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
        else if (status == LEAVING)
        {
            if (distance > 0)
            {
                if (direction)
                {
                    x += velocity;
                    man_following_girl_right.OnMove();
                }
                else
                {
                    x -= velocity;
                    man_following_girl_left.OnMove();
                }

                distance -= velocity;
            }
        }
    }

    void Man::OnShow(CGameMap* map)
    {
        if (status == ALIVE)
        {
            if (moving) //是否正在移動
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
        else if (status == ATTACKED_BY_MAINGIRL)
        {
            flash.SetTopLeft(map->ScreenX(x) - 18, map->ScreenY(y));
            flash.OnShow();
            weakening.SetTopLeft(map->ScreenX(x), map->ScreenY(y) + 5);
            weakening.OnShow();
            blood.SetTopLeft(map->ScreenX(x), map->ScreenY(y) - blood.Height());
            blood.OnShow();
        }
        else if (status == ATTACKED_BY_ALL)
        {
            flash_multi.SetTopLeft(map->ScreenX(x) - 40, map->ScreenY(y) - 25);
            flash_multi.OnShow();
            weakening.SetTopLeft(map->ScreenX(x), map->ScreenY(y) + 5);
            weakening.OnShow();
            clicking_bar.SetTopLeft(map->ScreenX(x) - 40, map->ScreenY(y));
            clicking_bar.ShowBitmap();
            color_point[0].x = clicking_bar.Left() + 3;
            color_point[0].y = clicking_bar.Top() + 3;
            color_point[1].x = int(clicking_bar.Left() + (abs(800 - HP) / 40) * 10.5);
            color_point[1].y = clicking_bar.Top() + clicking_bar.Height() - 3;
            DrawBeam(map);
            clicking.SetTopLeft(int(clicking_bar.Left() - clicking.Width() / 2 + (abs(800 - HP) / 40) * 10.5), map->ScreenY(y) - clicking.Height());
            clicking.OnShow();
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
        else if (status == FOLLOW)
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
        else if (status == LEAVING)
        {
            if (direction)
            {
                man_following_girl_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_following_girl_right.OnShow();
            }
            else
            {
                man_following_girl_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                man_following_girl_left.OnShow();
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
        return this->status != DEAD && this->status != FOLLOW && this->status != LEAVING;
    }

    bool Man::IsAlreadyDead()
    {
        return status == FOLLOW;
    }

    bool Man::IsOver()
    {
        return status == LEAVING && distance <= 0;
    }

    void Man::SetIsFocused(bool status)
    {
        is_focused = status;
    }

    void Man::SetIsAttackedBy(int who)
    {
        if (who == MAINGIRL)
            status = ATTACKED_BY_MAINGIRL;
        else if (who == ALL)
            status = ATTACKED_BY_ALL;
    }

    bool Man::IsAttackedBy(int who)
    {
        if (who == MAINGIRL && status == ATTACKED_BY_MAINGIRL)
            return true;
        else if (who == ALL && status == ATTACKED_BY_ALL)
            return true;

        return false;
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

    void Man::LoseHP(double value)
    {
        GAME_ASSERT(status == ATTACKED_BY_ALL || status == ATTACKED_BY_MAINGIRL, "Change Stat first!");
        double limit = 800;

        if (HP > limit)
            HP = limit;
        else if (limit >= HP && HP > 0)
            HP -= value;
        else
            HP = 0;

        //is_attacked = true;
    }

    void Man::SetIsKilledBy(int who)
    {
        GAME_ASSERT(status == DEAD, "KKKK");

        if (who == MAINGIRL)
            velocity = 20;
        else
            velocity = 5;

        is_killed_by = who;
    }

    void Man::Follow(int x, int y, bool direction)
    {
        GAME_ASSERT(status == FOLLOW, "Man doesn't want to follow someone.");
        fx = x;
        fy = y;
        fdirection = direction;
    }

    int Man::GetId()
    {
        return id;
    }

    double Man::GetHP()
    {
        return HP;
    }

    void Man::DrawBeam(CGameMap* map)
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CPen pen(PS_SOLID, 1, RGB(255, 0, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush brush(RGB(255, 51, 255));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        CRect rec;
        pDC->Rectangle(color_point[0].x, color_point[0].y, color_point[1].x, color_point[1].y);
        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    NormalMan::NormalMan(int x, int y, int start, int end, bool direction, int type) : Man(x, y, start, end, direction), type(type)
    {
        HP = 800;
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

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/Man/flash_multiple (" + to_string(i) + ").bmp").c_str());
            flash_multi.AddBitmap(text, RGB(0, 0, 0));
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

        if (!bitmapIsLoaded)
        {
            for (int i = 1; i <= 2; i++)
            {
                strcpy(text, ("RES/Man/clicking (" + to_string(i) + ").bmp").c_str());
                clicking.AddBitmap(text, RGB(0, 0, 0));
            }

            clicking.SetDelayCount(2);
            strcpy(text, "RES/Man/clicking_bar.bmp");
            clicking_bar.LoadBitmap(text, RGB(255, 255, 255));
            bitmapIsLoaded = true;
        }

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/follow/left/slave_girl (" + to_string(i) + ").bmp").c_str());
            man_following_girl_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/Man/normalMan" + to_string(type) + "/follow/right/slave_girl (" + to_string(i) + ").bmp").c_str());
            man_following_girl_right.AddBitmap(text, RGB(255, 255, 255));
        }

        man_following_girl_left.SetDelayCount(8);
        man_following_girl_right.SetDelayCount(8);
    }

}
