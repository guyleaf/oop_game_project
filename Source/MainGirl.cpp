#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    MainGirl::MainGirl() : x(450), y(MIDDLE), moving(false), velocity(5), is_focusing(false), is_attacking(false), is_locked(false), is_clicked(false)
    {
        is_bump = false;
        heartPoints = 0;
    }

    void MainGirl::LoadBitMap()
    {
        char text[100] = { 0 };
        girl_left_stand.LoadBitmap("RES/mainGirl/left/stand.bmp", RGB(0, 0, 0));
        girl_right_stand.LoadBitmap("RES/mainGirl/right/stand.bmp", RGB(0, 0, 0));
        girl_left_focusing_behind.LoadBitmap("RES/mainGirl/left/focusing_behind.bmp", RGB(230, 230, 196));
        girl_left_focusing_front.LoadBitmap("RES/mainGirl/left/focusing_front.bmp", RGB(0, 0, 0));
        girl_right_focusing_behind.LoadBitmap("RES/mainGirl/right/focusing_behind.bmp", RGB(230, 230, 196));
        girl_right_focusing_front.LoadBitmap("RES/mainGirl/right/focusing_front.bmp", RGB(0, 0, 0));

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/mainGirl/right/girl_walk (" + to_string(i) + ").bmp").c_str());
            girl_walk_right.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/mainGirl/left/girl_walk (" + to_string(i) + ").bmp").c_str());
            girl_walk_left.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/mainGirl/right/girl_run (" + to_string(i) + ").bmp").c_str());
            girl_run_right.AddBitmap(text, RGB(0, 0, 0));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/mainGirl/left/girl_run (" + to_string(i) + ").bmp").c_str());
            girl_run_left.AddBitmap(text, RGB(0, 0, 0));
        }

        for (int i = 1; i <= 3; i++)
        {
            strcpy(text, ("RES/mainGirl/focus_point_on (" + to_string(i) + ").bmp").c_str());
            focus_point_on.AddBitmap(text, RGB(230, 230, 196));
        }

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/mainGirl/focus_point_off (" + to_string(i) + ").bmp").c_str());
            focus_point_off.AddBitmap(text, RGB(230, 230, 196));
        }

        focus_point_on.SetDelayCount(1);
        focus_point_off.SetDelayCount(1);
        focus_point_on.SetTopLeft(-100, -100);
        focus_point_off.SetTopLeft(-100, -100);
        girl_run_left.SetDelayCount(5);
        girl_run_right.SetDelayCount(5);

        for (int i = 1; i <= 15; i++)
        {
            strcpy(text, ("RES/mainGirl/left/bump (" + to_string(i) + ").bmp").c_str());
            bump_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 15; i++)
        {
            strcpy(text, ("RES/mainGirl/right/bump (" + to_string(i) + ").bmp").c_str());
            bump_right.AddBitmap(text, RGB(255, 255, 255));
        }

        bump_left.SetDelayCount(6);
        bump_right.SetDelayCount(6);
    }

    void MainGirl::OnMove(CGameMap* map)
    {
        if (!is_attacking)
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
        else
        {
            focus_point_on.Reset();
            focus_point_off.Reset();
        }

        if (is_bump)
        {
            if (direction)
            {
                if (bump_right.GetCurrentBitmapNumber() <= 3)
                {
                    map->Addsx(-(9 - bump_right.GetCurrentBitmapNumber()));
                    x -= (9 - bump_right.GetCurrentBitmapNumber());
                    y += bump_right.GetCurrentBitmapNumber() + 1;
                }
                else if (bump_right.GetCurrentBitmapNumber() >= 11)
                {
                    map->Addsx((bump_right.GetCurrentBitmapNumber() - 5));
                    x += (bump_right.GetCurrentBitmapNumber() - 5);
                    y -= 15 - bump_right.GetCurrentBitmapNumber();
                }

                if (!bump_right.IsFinalBitmap())
                    bump_right.OnMove();
                else
                    is_bump = false;
            }
            else
            {
                if (bump_left.GetCurrentBitmapNumber() <= 3)
                {
                    map->Addsx((9 - bump_left.GetCurrentBitmapNumber()));
                    x += (9 - bump_left.GetCurrentBitmapNumber());
                    y += bump_left.GetCurrentBitmapNumber() + 1;
                }
                else if (bump_left.GetCurrentBitmapNumber() >= 11)
                {
                    map->Addsx(-(bump_left.GetCurrentBitmapNumber() - 5));
                    x -= (bump_left.GetCurrentBitmapNumber() - 5);
                    y -= 15 - bump_left.GetCurrentBitmapNumber();
                }

                if (!bump_left.IsFinalBitmap())
                    bump_left.OnMove();
                else
                    is_bump = false;
            }
        }
        else
        {
            bump_left.Reset();
            bump_right.Reset();

            if (!is_focusing)
            {
                if (cursor_x - (map->ScreenX(x) + girl_right_stand.Width()) > 0) //滑鼠座標與人物最右邊的座標相減(螢幕的點座標) 需大於0
                {
                    moving = true;
                    direction = true;
                }
                else if (map->ScreenX(x) - cursor_x > 0)
                {
                    moving = true;
                    direction = false;
                }
                else
                    moving = false;

                SetVelocity(map);
            }
            else
            {
                moving = false;

                if (!is_locked)
                {
                    //調整鎖定時的瞄準方向
                    if (map->ScreenX(x) + girl_left_stand.Width() / 2 <= cursor_x)
                        direction = true;
                    else
                        direction = false;
                }
            }

            if (!is_focusing && moving) //檢查是否正在移動
            {
                if (direction)
                {
                    if (map->Width() - (x + girl_right_stand.Width()) > 10)
                        x += velocity;

                    if (map->ScreenX(x) <= 150)
                        map->Addsx(velocity);
                    else
                        map->Addsx(35);
                }
                else
                {
                    if (x > 10)
                        x -= velocity;

                    if (map->ScreenX(x) >= 550)
                        map->Addsx(-velocity);
                    else
                        map->Addsx(-35);
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

        int sx;

        if (direction)
            sx = x - 30;
        else
            sx = x + girl_left_stand.Width() + 20;

        for (size_t i = 0; i < slaves.size(); i++)
        {
            if (direction)
            {
                sx -= (girl_left_stand.Width() + 8);
                slaves[i]->Follow(sx, y, direction);
            }
            else
            {
                slaves[i]->Follow(sx, y, direction);
                sx += (girl_left_stand.Width() + 8);
            }

            slaves[i]->OnMove();
        }
    }

    void MainGirl::OnMouseMove(CGameMap* map, CPoint point)
    {
        cursor_x = point.x;
        cursor_y = point.y;
    }

    void MainGirl::SetVelocity(CGameMap* map)
    {
        if (!moving) //沒有正在移動則退出
            return;

        int distance; //滑鼠的螢幕點座標到人物的距離

        if (direction) //false => 往左, true => 往右
            distance = cursor_x - (map->ScreenX(x) + girl_right_stand.Width());
        else
            distance = map->ScreenX(x) - cursor_x;

        if (distance > 300) //距離越遠速度越快
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


    void MainGirl::OnShow(CGameMap* map)
    {
        if (is_bump)
        {
            if (direction)
            {
                bump_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                bump_right.OnShow();
            }
            else
            {
                bump_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                bump_left.OnShow();
            }
        }
        else if (is_locked)
        {
            if (direction)
            {
                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= beam_pos[0].y)
                {
                    girl_right_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_front.ShowBitmap();

                    if (is_attacking)
                        DrawBeam(map);
                }
                else
                {
                    if (is_attacking)
                        DrawBeam(map);

                    girl_right_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_behind.ShowBitmap();
                }
            }
            else
            {
                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= beam_pos[0].y)
                {
                    girl_left_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_front.ShowBitmap();

                    if (is_attacking)
                        DrawBeam(map);
                }
                else
                {
                    if (is_attacking)
                        DrawBeam(map);

                    girl_left_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_behind.ShowBitmap();
                }
            }
        }
        else if (is_focusing)
        {
            if (direction)
            {
                beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 40, map->ScreenY(y) + girl_left_stand.Height() / 5);
                beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2, map->ScreenY(y) + girl_left_stand.Height() / 5);

                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y)
                {
                    girl_right_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_front.ShowBitmap();

                    if (is_attacking)
                        DrawBeam(map);
                }
                else
                {
                    if (is_attacking)
                        DrawBeam(map);

                    girl_right_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_right_focusing_behind.ShowBitmap();
                }
            }
            else
            {
                beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 10, map->ScreenY(y) + girl_left_stand.Height() / 5);
                beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 - 20, map->ScreenY(y) + girl_left_stand.Height() / 5);

                if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y)
                {
                    girl_left_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_front.ShowBitmap();

                    if (is_attacking)
                        DrawBeam(map);
                }
                else
                {
                    if (is_attacking)
                        DrawBeam(map);

                    girl_left_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    girl_left_focusing_behind.ShowBitmap();
                }
            }
        }
        else if (moving) //是否正在移動
        {
            if (direction) //false => 往左, true => 往右
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

        for (size_t i = 0; i < slaves.size(); i++)
        {
            slaves[i]->OnShow(map);
        }
    }

    void MainGirl::Attack(Man* man, CGameMap* map)
    {
        beam_pos[0].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 - 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        beam_pos[1].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 + 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);

        if (!is_locked)
            man->LoseHP(15);
        else
            man->LoseHP(70);

        if (is_locked)
            is_clicked = false;
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

    bool MainGirl::IsFocusPerson(Man* man)
    {
        return focus_id == man->GetId();
    }

    void MainGirl::SetFocusPerson(CGameMap* map, Man* man)
    {
        focus_point_on.SetTopLeft(map->ScreenX(man->GetX()) + 10, map->ScreenY(man->GetY()) + 3);
        focus_point_off.SetTopLeft(map->ScreenX(man->GetX()) + 10, map->ScreenY(man->GetY()) + 3);
        focus_id = man->GetId();
    }

    void MainGirl::ShowFocus()
    {
        if (!is_attacking && !is_bump)
        {
            if (is_focusing)
            {
                focus_point_on.OnShow();
            }
            else
            {
                //避免電死男生時，準星出現的情況
                if (!focus_point_off.IsFinalBitmap() && (slaves.size() == 0 || (*(slaves.end() - 1))->GetId() != focus_id))
                    focus_point_off.OnShow();
            }
        }
    }

    void MainGirl::SetIsAttacking(bool status)
    {
        is_attacking = status;
    }

    void MainGirl::SetIsLocked(bool status)
    {
        is_locked = status;
    }

    bool MainGirl::IsLocked()
    {
        return is_locked;
    }
    bool MainGirl::IsAttacking()
    {
        return is_attacking;
    }

    void MainGirl::Lose()
    {
        is_bump = true;
    }

    void MainGirl::Click()
    {
        is_clicked = true;
    }

    bool MainGirl::IsClicked()
    {
        return is_clicked;
    }

    void MainGirl::AddSlave(Man* man)
    {
        slaves.insert(slaves.begin(), man);
    }

    int MainGirl::GetHeartPoints()
    {
        return heartPoints;
    }

    void MainGirl::DrawBeam(CGameMap* map)
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

    int MainGirl::GetPositionX()
    {
        return x;
    }

    int MainGirl::GetPositionY()
    {
        return y;
    }

    int MainGirl::Height()
    {
        return girl_left_stand.Height();
    }

    int MainGirl::Width()
    {
        return girl_left_stand.Width();
    }
}