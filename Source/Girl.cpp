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
        ATTACKING,
        WIN,
        FLYING,
        LEAVING,
        DEAD
    };

    vector<Man*> Girl::slaves;

    Girl::Girl(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(5)
    {
        range[0] = start;
        range[1] = end;
        is_shocking = false;
        status = ALIVE;
    }

    void Girl::SetDirection(bool direction)
    {
        this->direction = direction;
    }

    void Girl::SetMoving(bool status)
    {
        this->moving = status;
    }

    void Girl::OnMove(CGameMap* map)
    {
        if (status == ALIVE)
        {
            if (is_shocking)
            {
                exclamation.OnMove();

                if (direction)
                    surprising_right.OnMove();
                else
                    surprising_left.OnMove();
            }

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
                    girl_left.OnMove();
            }
        }
        else if (status == ATTACKING)
        {
        }
        else if (status == FLYING)
        {
            if (map->IsInScreen(x, x + girl_left_stand.Width()))
                if (direction)
                {
                    x -= 15;
                    y -= 15;
                    flying_right.OnMove();
                }
                else
                {
                    x += 15;
                    y -= 15;
                    flying_left.OnMove();
                }
            else
                status = DEAD;
        }
    }

    void Girl::OnShow(CGameMap* map)
    {
        if (status == ALIVE)
        {
            if (is_shocking)
            {
                exclamation.SetTopLeft(map->ScreenX(x) + girl_left_stand.Width() / 2, map->ScreenY(y) - exclamation.Height());
                exclamation.OnShow();

                if (direction)
                {
                    beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 15, map->ScreenY(y) + girl_left_stand.Height() / 5 - 10);
                    beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 5, map->ScreenY(y) + girl_left_stand.Height() / 5 + 10);
                    notice_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    notice_right.ShowBitmap();
                    surprising_right.SetTopLeft(map->ScreenX(x + girl_left_stand.Width()), map->ScreenY(y));
                    surprising_right.OnShow();
                }
                else
                {
                    beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 - 20, map->ScreenY(y) + girl_left_stand.Height() / 5 - 10);
                    beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 - 5, map->ScreenY(y) + girl_left_stand.Height() / 5 + 10);
                    notice_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    notice_left.ShowBitmap();
                    surprising_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    surprising_left.OnShow();
                }
            }
            else
            {
                exclamation.Reset();
                surprising_right.Reset();
                surprising_left.Reset();

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
        else if (status == ATTACKING)
        {
            if (direction)
            {
                if (map->ScreenX(y) <= map->Height() / 2)
                {
                    shooting_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    shooting_right.ShowBitmap();
                    DrawBeam(map);
                }
                else
                {
                    DrawBeam(map);
                    shooting_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    shooting_right.ShowBitmap();
                }
            }
            else
            {
                if (map->ScreenX(y) <= map->Height() / 2)
                {
                    shooting_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    shooting_left.ShowBitmap();
                    DrawBeam(map);
                }
                else
                {
                    DrawBeam(map);
                    shooting_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    shooting_left.ShowBitmap();
                }
            }
        }
        else if (status == FLYING)
        {
            if (direction)
            {
                flying_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                flying_right.OnShow();
            }
            else
            {
                flying_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                flying_left.OnShow();
            }
        }
    }

    void Girl::SetIsShocking(bool status)
    {
        is_shocking = status;
    }

    bool Girl::IsShocking()
    {
        return is_shocking;
    }

    void Girl::LockPerson(Man* man, CGameMap* map)
    {
        beam_pos[0].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 - 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        beam_pos[1].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 + 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
    }

    void Girl::Attack(Man* man, CGameMap* map)
    {
        man->LoseHP(-3);
        status = ATTACKING;
    }


    bool Girl::IsLocked()
    {
        return exclamation.IsFinalBitmap();
    }

    void Girl::Win()
    {
        status = WIN;
    }

    void Girl::Lose()
    {
        status = FLYING;
        CAudio::Instance()->Play(AUDIO_FLYING, false);
    }

    bool Girl::IsWinner()
    {
        return status == WIN;
    }

    bool Girl::IsAlreadyDead()
    {
        return status == DEAD;
    }

    int Girl::GetX()
    {
        return x;
    }

    int Girl::GetY()
    {
        return y;
    }

    int Girl::GetWidth()
    {
        return girl_left_stand.Width();
    }

    int Girl::GetHeight()
    {
        return girl_left_stand.Height();
    }

    void Girl::AddSlave(Man* man)
    {
    }

    void Girl::DrawBeam(CGameMap* map)
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CPen pen(PS_SOLID, 1, RGB(255, 0, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush brush(RGB(255, 51, 255));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Polygon(beam_pos, 4);
        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
    ////////////////////////////////////////////////////////////////////////////////////

    NormalGirl::NormalGirl(int x, int y, int start, int end, bool direction, int type) : Girl(x, y, start, end, direction), type(type)
    {
    }

    void NormalGirl::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/stand.bmp").c_str());
        girl_left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/stand.bmp").c_str());
        girl_right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/normalGirl (" + to_string(i) + ").bmp").c_str());
            girl_right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/normalGirl (" + to_string(i) + ").bmp").c_str());
            girl_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i < 5; i++)
        {
            strcpy(text, ("RES/Girl/exclamation (" + to_string(i) + ").bmp").c_str());
            exclamation.AddBitmap(text, RGB(255, 255, 255));
        }

        exclamation.SetDelayCount(4);
        strcpy(text, "RES/Girl/surprising_left.bmp");
        surprising_left.AddBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/blank.bmp");
        surprising_left.AddBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/surprising_right.bmp");
        surprising_right.AddBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/blank.bmp");
        surprising_right.AddBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/flying (" + to_string(i) + ").bmp").c_str());
            flying_left.AddBitmap(text, RGB(255, 255, 255));
        }

        flying_left.SetDelayCount(1);

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/flying (" + to_string(i) + ").bmp").c_str());
            flying_right.AddBitmap(text, RGB(255, 255, 255));
        }

        flying_right.SetDelayCount(1);
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/shooting.bmp").c_str());
        shooting_left.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/shooting.bmp").c_str());
        shooting_right.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/notice.bmp").c_str());
        notice_left.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/notice.bmp").c_str());
        notice_right.LoadBitmap(text, RGB(255, 255, 255));
    }
}