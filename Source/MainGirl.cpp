#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    MainGirl::MainGirl() : x(450), y(220), moving(false), velocity(5), is_focusing(false), is_attacking(false)
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

        if (moving) //檢查是否正在移動
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
        if (!moving) //沒有正在移動則退出
            return;

        int distance; //滑鼠的螢幕點座標到人物的距離

        if (direction) //false => 往左, true => 往右
            distance = point.x - (map->ScreenX(x) + girl_right_stand.Width());
        else
            distance = map->ScreenX(x) - point.x;

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

    void MainGirl::SetMoving(CGameMap* map, CPoint point)
    {
        if (point.x - (map->ScreenX(x) + girl_right_stand.Width()) > 0) //滑鼠座標與人物最右邊的座標相減(螢幕的點座標) 需大於0
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
        if (status == false)
            focus_id = 0;

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
        beam_pos[0].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 - 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        beam_pos[1].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 + 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        focus_id = man->GetId();
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

    void MainGirl::SetIsAttacking(bool status)
    {
        is_attacking = status;
    }

    bool MainGirl::IsAttacking()
    {
        return is_attacking;
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
}