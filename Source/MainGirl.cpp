#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum state
    {
        INNORMAL, // 正常狀態
        INANIMATION // 過場動畫
    };

    MainGirl::MainGirl()
    {
    }

    MainGirl::~MainGirl()
    {
    }

    void MainGirl::OnBeginState() // 初始化狀態
    {
        is_bump = false;
        is_reinforced = false;
        state = INNORMAL;
        delay_counter = 30;
        cursor_x = 450;
        cursor_y = 0;
        is_reporting = false;
        x = 450;
        y = MIDDLE;
        ny = MIDDLE;
        moving = false;
        velocity = 5;
        is_focusing = false;
        is_attacking = false;
        is_locked = false;
        is_clicked = false;
        is_interrupted = false;
        slaves.erase(slaves.begin(), slaves.end());
        focus_point_on.SetTopLeft(-100, -100);
        focus_point_off.SetTopLeft(-100, -100);
        focus_point_on.Reset();
        focus_point_off.Reset();
        bump_left.Reset();
        bump_right.Reset();
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

        for (int i = 1; i <= 19; i++)
        {
            strcpy(text, ("RES/mainGirl/left/bump (" + to_string(i) + ").bmp").c_str());
            bump_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 19; i++)
        {
            strcpy(text, ("RES/mainGirl/right/bump (" + to_string(i) + ").bmp").c_str());
            bump_right.AddBitmap(text, RGB(255, 255, 255));
        }

        bump_left.SetDelayCount(6);
        bump_right.SetDelayCount(6);

        for (int i = 1; i <= 32; i++)
        {
            strcpy(text, ("RES/mainGirl/right/reinforcing (" + to_string(i) + ").bmp").c_str());
            girl_right_reinforcing.AddBitmap(text, RGB(0, 0, 0));
        }

        girl_right_reinforcing.SetDelayCount(5);

        for (int i = 1; i <= 32; i++)
        {
            strcpy(text, ("RES/mainGirl/left/reinforcing (" + to_string(i) + ").bmp").c_str());
            girl_left_reinforcing.AddBitmap(text, RGB(0, 0, 0));
        }

        girl_left_reinforcing.SetDelayCount(5);

        for (int i = 1; i <= 13; i++)
        {
            strcpy(text, ("RES/mainGirl/reinforcing (" + to_string(i) + ").bmp").c_str());
            reinforcing[0].AddBitmap(text, RGB(230, 230, 196));
            reinforcing[1].AddBitmap(text, RGB(230, 230, 196));
        }

        reinforcing[0].SetDelayCount(5);
        reinforcing[1].SetDelayCount(5);
        InitializeReinforcing();
        surprising_left.AddBitmap("RES/mainGirl/surprising_left.bmp", RGB(255, 255, 255));
        surprising_left.AddBitmap("RES/mainGirl/blank.bmp", RGB(255, 255, 255));
        surprising_right.AddBitmap("RES/mainGirl/surprising_right.bmp", RGB(255, 255, 255));
        surprising_right.AddBitmap("RES/mainGirl/blank.bmp", RGB(255, 255, 255));

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/mainGirl/fun (" + to_string(i) + ").bmp").c_str());
            fun.AddBitmap(text, RGB(255, 255, 255));
        }

        fun.SetDelayCount(7);
        over_notice_left.LoadBitmap("RES/mainGirl/left/over_notice.bmp", RGB(0, 0, 0));
        over_notice_right.LoadBitmap("RES/mainGirl/right/over_notice.bmp", RGB(0, 0, 0));

        for (int i = 1; i <= 9; i++)
        {
            strcpy(text, ("RES/mainGirl/left/over_walking (" + to_string(i) + ").bmp").c_str());
            over_walking_left .AddBitmap(text, RGB(0, 0, 0));
        }

        over_walking_left.SetDelayCount(5);

        for (int i = 1; i <= 9; i++)
        {
            strcpy(text, ("RES/mainGirl/right/over_walking (" + to_string(i) + ").bmp").c_str());
            over_walking_right.AddBitmap(text, RGB(0, 0, 0));
        }

        over_walking_right.SetDelayCount(5);
    }

    /*
    	遊戲結束方法有兩種
    		1. 擊飛時，heart沒了 (死亡)
    		2. 時間到，進行結算

    	is_interrupted的詳細使用用途
    	用於準星顯示，是否被中斷
    	以下變換狀態時，會設定為true
    		1. 被擊飛時
    		2. 進入特殊時間過場動畫
    */
    void MainGirl::OnMove(CGameMap* map, UI* ui)
    {
        if (state == INANIMATION)
        {
            if (ui->IsGameOver() && ui->GetHeartPoints() > 0) // 結算動作
            {
                static int count = 40;
                static unsigned int startIndex = 0;

                if (moving) // 進行結算移動
                {
                    if (direction)
                    {
                        x += 5;
                        over_walking_right.OnMove();
                    }
                    else
                    {
                        x -= 5;
                        over_walking_left.OnMove();
                    }

                    fun.OnMove();

                    if (count <= 0 && startIndex < slaves.size()) // 奴隸總分結算
                    {
                        if (startIndex == 0)
                        {
                            slaves[startIndex]->Report();
                            ui->AddScore(slaves[startIndex]->GetScore());
                            startIndex++;
                        }
                        else if (startIndex != 0 && !slaves[startIndex - 1]->IsReporting())
                        {
                            slaves[startIndex]->Report();
                            ui->AddScore(slaves[startIndex]->GetScore());
                            startIndex++;
                        }

                        if (startIndex == slaves.size())
                            count = 30;
                    }

                    count--;

                    if (startIndex >= slaves.size() && count <= 0)
                        is_reporting = false;
                }
                else // 站立
                {
                    if (count > 0) // 注意鐘聲
                    {
                        if (direction)
                        {
                            surprising_right.OnMove();
                        }
                        else
                        {
                            surprising_left.OnMove();
                        }

                        count--;

                        if (count == 0)
                        {
                            startIndex = 0;
                            moving = true;
                            count = 50;
                        }
                    }
                    else
                        count = 40;
                }
            }
            else if (is_bump) // 擊飛狀態
            {
                // 負責擊飛狀態的移動軌跡
                if (direction)
                {
                    if (bump_right.GetCurrentBitmapNumber() <= 3)
                    {
                        map->Addsx(-(9 - bump_right.GetCurrentBitmapNumber()));
                        x -= (9 - bump_right.GetCurrentBitmapNumber());
                        y += bump_right.GetCurrentBitmapNumber() + 1;
                    }
                    else if (bump_right.GetCurrentBitmapNumber() == 14)
                    {
                        map->Addsx((bump_right.GetCurrentBitmapNumber() - 5));
                        x += (bump_right.GetCurrentBitmapNumber() - 5);
                    }
                    else if (bump_right.GetCurrentBitmapNumber() > 14)
                    {
                        map->Addsx((bump_right.GetCurrentBitmapNumber() - 5));
                        x += (bump_right.GetCurrentBitmapNumber() - 5);
                        y -= 19 - bump_right.GetCurrentBitmapNumber();
                    }
                }
                else
                {
                    if (bump_left.GetCurrentBitmapNumber() <= 3)
                    {
                        map->Addsx((9 - bump_left.GetCurrentBitmapNumber()));
                        x += (9 - bump_left.GetCurrentBitmapNumber());
                        y += bump_left.GetCurrentBitmapNumber() + 1;
                    }
                    else if (bump_left.GetCurrentBitmapNumber() == 14)
                    {
                        map->Addsx(-(bump_left.GetCurrentBitmapNumber() - 5));
                        x -= (bump_left.GetCurrentBitmapNumber() - 5);
                    }
                    else if (bump_left.GetCurrentBitmapNumber() > 14)
                    {
                        map->Addsx(-(bump_left.GetCurrentBitmapNumber() - 5));
                        x -= (bump_left.GetCurrentBitmapNumber() - 5);
                        y -= 19 - bump_left.GetCurrentBitmapNumber();
                    }
                }

                // 負責更新擊飛動畫
                if (ui->GetHeartPoints() > 0)
                {
                    if (direction)
                    {
                        if (!bump_right.IsFinalBitmap())
                            bump_right.OnMove();
                        else // 結束後，跳回正常狀態
                        {
                            is_bump = false;
                            state = INNORMAL;
                        }
                    }
                    else
                    {
                        if (!bump_left.IsFinalBitmap())
                            bump_left.OnMove();
                        else // 結束後，跳回正常狀態
                        {
                            is_bump = false;
                            state = INNORMAL;
                        }
                    }
                }
                else // 擊飛過程中，heart為空時，則播放死亡音效並進入結束
                {
                    if (bump_right.GetCurrentBitmapNumber() <= 11 && bump_left.GetCurrentBitmapNumber() <= 11)
                    {
                        if (direction)
                            bump_right.OnMove();
                        else
                            bump_left.OnMove();

                        if (bump_right.GetCurrentBitmapNumber() == 12 || bump_left.GetCurrentBitmapNumber() == 12)
                        {
                            ui->SetIsGameOver(true);
                            CAudio::Instance()->Pause();
                            CAudio::Instance()->Play(AUDIO_LOSE);
                        }
                    }
                }
            }
            else if (is_reinforced) // 特殊時間
            {
                if (direction)
                {
                    if (!girl_right_reinforcing.IsFinalBitmap())
                    {
                        girl_right_reinforcing.OnMove();

                        if (girl_right_reinforcing.GetCurrentBitmapNumber() == 20) // 特殊時間變身音效
                            CAudio::Instance()->Play(AUDIO_BLINK, false);
                    }
                    else // 結束後，跳回正常狀態
                    {
                        state = INNORMAL;
                        CAudio::Instance()->Stop(AUDIO_FLYING);
                        CAudio::Instance()->Resume();
                    }
                }
                else
                {
                    if (!girl_left_reinforcing.IsFinalBitmap())
                    {
                        girl_left_reinforcing.OnMove();

                        if (girl_left_reinforcing.GetCurrentBitmapNumber() == 20) // 特殊時間變身音效
                            CAudio::Instance()->Play(AUDIO_BLINK, false);
                    }
                    else // 結束後，跳回正常狀態
                    {
                        state = INNORMAL;
                        CAudio::Instance()->Stop(AUDIO_FLYING);
                        CAudio::Instance()->Resume();
                    }
                }
            }
        }
        else if (state == INNORMAL)
        {
            if (is_reinforced) // 特殊時間的周圍特效
            {
                reinforcing[0].OnMove();

                if (delay_counter < 0)
                    reinforcing[1].OnMove();

                if (delay_counter >= 0)
                    delay_counter--;
            }
            else
            {
                girl_left_reinforcing.Reset();
                girl_right_reinforcing.Reset();
            }

            if (!is_attacking) // 準星動畫
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
                is_interrupted = false;
                focus_point_on.Reset();
                focus_point_off.Reset();
            }

            bump_left.Reset();
            bump_right.Reset();

            if (!is_focusing) // 非鎖定狀態，用於判斷走路部分
            {
                if (cursor_x - (map->ScreenX(x) + girl_right_stand.Width()) > 0) // 滑鼠座標與人物最右邊的座標相減(螢幕的點座標) 需大於0
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

                if (!map->IsEmpty(x, y) && map->IsEmpty(x + Width(), y) && direction == false)
                    moving = false;
                else if (map->IsEmpty(x, y) && !map->IsEmpty(x + Width(), y) && direction == true)
                    moving = false;

                SetVelocity(map);
            }
            else // 鎖定狀態
            {
                moving = false;

                if (!is_locked) // 尚未進入鎖死前，依游標位置決定面向左右
                {
                    // 調整鎖定時的瞄準方向
                    if (map->ScreenX(x) + girl_left_stand.Width() / 2 <= cursor_x)
                        direction = true;
                    else
                        direction = false;
                }
            }

            if (moving)  // 檢查是否正在移動
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

            if (ui->IsGameOver() && ui->GetHeartPoints() > 0) // 進入結算動作
            {
                is_reporting = true;
                state = INANIMATION;
                moving = false;
            }
        }

        int sx;

        if (direction)
            sx = x - 30;
        else
        {
            sx = x + girl_left_stand.Width() + 38;
        }

        for (size_t i = 0; i < slaves.size(); i++) // 命令奴隸跟著指定座標
        {
            if (direction)
            {
                sx -= (girl_left_stand.Width() + 8);

                if (is_bump)
                    slaves[i]->Follow(sx, ny, direction);
                else
                    slaves[i]->Follow(sx, y, direction);
            }
            else
            {
                if (is_bump)
                    slaves[i]->Follow(sx, ny, direction);
                else
                    slaves[i]->Follow(sx, y, direction);

                sx += (girl_left_stand.Width() + 8);
            }

            slaves[i]->OnMove(0);
        }
    }

    void MainGirl::OnMouseMove(CPoint point) // 處理滑鼠移動
    {
        cursor_x = point.x;
        cursor_y = point.y;
    }

    void MainGirl::InitializeReinforcing() // 初始化特殊時間參數
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CDC ImageDC;
        ImageDC.CreateCompatibleDC(pDC);
        ImageDC.SetBkColor(RGB(0, 0, 0));
        m_memBitmap.CreateCompatibleBitmap(pDC, 140, 500);
        CBitmap* pOldBitmap = ImageDC.SelectObject(&m_memBitmap);
        CPen pen(PS_SOLID, 3, RGB(255, 0, 255));
        CBrush brush(RGB(255, 214, 255));
        CPen linePen(PS_SOLID, 4, RGB(255, 214, 255));
        CPen* pOldPen = ImageDC.SelectObject(&pen);
        CBrush* pOldBrush = ImageDC.SelectObject(&brush);
        CRect Erect, Rrect;
        Erect.SetRect(0, 452, 135, 467);
        ImageDC.Ellipse(&Erect);
        Rrect.SetRect(0, -10, 135, 457);
        ImageDC.Rectangle(&Rrect);
        ImageDC.SelectObject(pOldPen);
        ImageDC.SelectObject(pOldBrush);
        // cover rectangle border
        CPen* pOldLinePen = ImageDC.SelectObject(&linePen);
        ImageDC.MoveTo(4, 456);
        ImageDC.LineTo(131, 456);
        ImageDC.SelectObject(pOldLinePen);
        ImageDC.SelectObject(pOldBitmap);
        ImageDC.DeleteDC();
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }

    void MainGirl::SetVelocity(CGameMap* map) // 設定移動速度
    {
        if (!moving) // 沒有正在移動則退出
            return;

        int distance; // 滑鼠的螢幕點座標到人物的距離

        if (direction) // false => 往左, true => 往右
            distance = cursor_x - (map->ScreenX(x) + girl_right_stand.Width());
        else
            distance = map->ScreenX(x) - cursor_x;

        if (distance > 300) // 距離越遠速度越快
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

    void MainGirl::OnShow(CGameMap* map, UI* ui)
    {
        if (state == INANIMATION)
        {
            if (ui->IsGameOver() && ui->GetHeartPoints() > 0) // 結算動作
            {
                if (moving)
                {
                    if (direction)
                    {
                        over_walking_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        over_walking_right.OnShow();
                    }
                    else
                    {
                        over_walking_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        over_walking_left.OnShow();
                    }

                    fun.SetTopLeft(map->ScreenX(x) + 12, map->ScreenY(y) - 5);
                    fun.OnShow();
                }
                else
                {
                    if (direction) //false => 往左, true => 往右
                    {
                        over_notice_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        over_notice_right.ShowBitmap();
                        surprising_right.SetTopLeft(map->ScreenX(x + over_notice_right.Width()) - 5, map->ScreenY(y) - 5);
                        surprising_right.OnShow();
                    }
                    else
                    {
                        over_notice_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        over_notice_left.ShowBitmap();
                        surprising_left.SetTopLeft(map->ScreenX(x) - 22, map->ScreenY(y) - 5);
                        surprising_left.OnShow();
                    }
                }
            }
            else if (is_bump) // 擊飛畫面
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
            else if (is_reinforced) // 特殊時間
            {
                // 20張以前顯示變身光束
                if (girl_right_reinforcing.GetCurrentBitmapNumber() < 20 && girl_left_reinforcing.GetCurrentBitmapNumber() < 20)
                {
                    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
                    CDC ImageDC;
                    ImageDC.CreateCompatibleDC(pDC);
                    CBitmap* pOldBitmap = ImageDC.SelectObject(&m_memBitmap);
                    CPoint coordinates[2];
                    coordinates[0].SetPoint(map->ScreenX(x - 30), 0);
                    coordinates[1].SetPoint(map->ScreenX(x + girl_left_stand.Width() + 30), map->ScreenY(y + girl_left_stand.Height() + 5));
                    CRect Erect, Rrect;
                    Erect.SetRect(0, 452, 135, 467);
                    Rrect.SetRect(0, -10, 135, 457);
                    BLENDFUNCTION bf;
                    bf.AlphaFormat = 0;
                    bf.BlendFlags = 0;
                    bf.BlendOp = 0;
                    bf.SourceConstantAlpha = 220;
                    pDC->AlphaBlend(coordinates[0].x, coordinates[0].y, Rrect.Width(), Rrect.Height() + Erect.Height() - 14, &ImageDC, 0, 0, Rrect.Width(), Rrect.Height() + Erect.Height() - 14, bf);
                    //pDC->BitBlt(coordinates[0].x, coordinates[0].y, Rrect.Width(), Rrect.Height() + Erect.Height() - 94, &ImageDC, 0, 80, SRCCOPY);
                    ImageDC.SelectObject(pOldBitmap);
                    ImageDC.DeleteDC();
                    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
                }

                if (direction)
                {
                    girl_right_reinforcing.SetTopLeft(map->ScreenX(x - 170), map->ScreenY(y + girl_left_stand.Height() - girl_right_reinforcing.Height()));
                    girl_right_reinforcing.OnShow();
                }
                else
                {
                    girl_left_reinforcing.SetTopLeft(map->ScreenX(x - 170), map->ScreenY(y + girl_left_stand.Height() - girl_right_reinforcing.Height()));
                    girl_left_reinforcing.OnShow();
                }
            }
        }
        else if (state == INNORMAL)
        {
            if (is_locked) // 鎖定狀態 (與其他女生搶)
            {
                if (direction) // 右邊
                {
                    if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= beam_pos[0].y) // 下方
                    {
                        girl_right_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_right_focusing_front.ShowBitmap();

                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);
                    }
                    else // 上方
                    {
                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);

                        girl_right_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_right_focusing_behind.ShowBitmap();
                    }
                }
                else // 左邊
                {
                    if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= beam_pos[0].y) // 下方
                    {
                        girl_left_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_left_focusing_front.ShowBitmap();

                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);
                    }
                    else // 上方
                    {
                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);

                        girl_left_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_left_focusing_behind.ShowBitmap();
                    }
                }
            }
            else if (is_focusing) // 單人鎖定狀態
            {
                if (direction) // 右邊
                {
                    // 設定光束末端位置
                    beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 40, map->ScreenY(y) + girl_left_stand.Height() / 5);
                    beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2, map->ScreenY(y) + girl_left_stand.Height() / 5);

                    if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y) // 下方
                    {
                        girl_right_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_right_focusing_front.ShowBitmap();

                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);
                    }
                    else // 上方
                    {
                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);

                        girl_right_focusing_behind.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_right_focusing_behind.ShowBitmap();
                    }
                }
                else // 左邊
                {
                    // 設定光束末端位置
                    beam_pos[2].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 + 10, map->ScreenY(y) + girl_left_stand.Height() / 5);
                    beam_pos[3].SetPoint(map->ScreenX(x) + girl_left_stand.Width() / 2 - 20, map->ScreenY(y) + girl_left_stand.Height() / 5);

                    if (map->ScreenY(y) + girl_left_stand.Height() / 2 <= cursor_y) // 下方
                    {
                        girl_left_focusing_front.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_left_focusing_front.ShowBitmap();

                        if (is_attacking) // 攻擊時顯示雷射
                            DrawBeam(map);
                    }
                    else // 上方
                    {
                        if (is_attacking) // 攻擊時顯示雷射
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
                    if (velocity != 12) // 走路
                    {
                        girl_walk_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_walk_right.OnShow();
                    }
                    else // 跑步
                    {
                        girl_run_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_run_right.OnShow();
                    }
                }
                else
                {
                    if (velocity != 12) // 走路
                    {
                        girl_walk_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_walk_left.OnShow();
                    }
                    else // 跑步
                    {
                        girl_run_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        girl_run_left.OnShow();
                    }
                }
            }
            else // 站力
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

            if (is_reinforced) // 特殊時間周圍效果
            {
                reinforcing[0].SetTopLeft(map->ScreenX(x - girl_left_stand.Width() / 2), map->ScreenY(y + 80));
                reinforcing[0].OnShow();

                if (delay_counter < 0)
                {
                    reinforcing[1].SetTopLeft(map->ScreenX(x - girl_left_stand.Width() / 2), map->ScreenY(y + 20));
                    reinforcing[1].OnShow();
                }
            }
        }

        for (size_t i = 0; i < slaves.size(); i++)
        {
            slaves[i]->OnShow(map);
        }
    }

    void MainGirl::Attack(Man* man, CGameMap* map)
    {
        // 設定雷射發射端位置
        beam_pos[0].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 - 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        beam_pos[1].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 + 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);

        if (!is_locked) // 單人攻擊時
        {
            if (is_reinforced)
                man->LoseHP(50);
            else
                man->LoseHP(15);
        }
        else // 鎖定攻擊時 (與其他女生搶)
        {
            if (is_reinforced)
                man->LoseHP(100);
            else
                man->LoseHP(70);
        }

        if (is_locked) // 鎖定狀態時，每點一次攻擊一次，功能為解除觸發
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

    void MainGirl::SetIsFocusing(bool status) // 設定鎖定狀態 (單人)
    {
        is_focusing = status;
    }

    bool MainGirl::IsFocusing() // 是否鎖定
    {
        return is_focusing;
    }

    bool MainGirl::IsFocusPerson(Man* man) // 是否為鎖定對象
    {
        return focus_id == man->GetId();
    }

    void MainGirl::SetFocusPerson(CGameMap* map, Man* man) // 設定鎖定的男生
    {
        focus_point_on.SetTopLeft(map->ScreenX(man->GetX()) + 10, map->ScreenY(man->GetY()) + 3);
        focus_point_off.SetTopLeft(map->ScreenX(man->GetX()) + 10, map->ScreenY(man->GetY()) + 3);
        focus_id = man->GetId();
    }

    void MainGirl::ShowFocus() // 顯示準星
    {
        if (!is_attacking && !is_bump && state != INANIMATION) // 無過場動畫/擊飛/不在攻擊狀態時
        {
            if (is_focusing) // 顯示鎖定準星動畫
            {
                focus_point_on.OnShow();
            }
            else if (!is_interrupted && !focus_point_off.IsFinalBitmap()) // 顯示收準星動畫
            {
                focus_point_off.OnShow();
            }
        }
    }

    void MainGirl::SetIsAttacking(bool status) // 設定攻擊狀態
    {
        is_attacking = status;
    }

    void MainGirl::SetIsLocked(bool status) // 設定鎖死狀態 (與其他女生搶)
    {
        is_locked = status;
    }

    void MainGirl::SetIsReinforced(bool status) // 設定特殊時間模式
    {
        is_reinforced = status;

        if (status == true)
        {
            is_interrupted = true; // 設定攻擊中斷
            state = INANIMATION;
        }
    }

    bool MainGirl::IsInAnimation() // 是否正在過場動畫
    {
        return state == INANIMATION;
    }

    bool MainGirl::IsReinforced() // 是否是特殊時間模式
    {
        return is_reinforced;
    }

    bool MainGirl::IsLocked() // 是否鎖死
    {
        return is_locked;
    }
    bool MainGirl::IsAttacking() // 是否正在攻擊
    {
        return is_attacking;
    }

    bool MainGirl::IsReporting() // 是否正在結算分數
    {
        return is_reporting;
    }

    void MainGirl::Lose() // 輸掉
    {
        is_bump = true;
        focus_id = -1;
        ny = y;
        is_interrupted = true;
        state = INANIMATION;
    }

    void MainGirl::Click() // 鎖死狀態時生效，每按一次攻擊一次
    {
        is_clicked = true;
    }

    bool MainGirl::IsClicked() // 鎖定狀態時生效，是否按下滑鼠
    {
        return is_clicked;
    }

    void MainGirl::AddSlave(Man* man) // 將搶到的男生，加入奴隸行列
    {
        slaves.insert(slaves.begin(), man);
    }

    void MainGirl::DrawBeam(CGameMap* map) // 畫出雷射
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