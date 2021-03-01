#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum State // 狀態
    {
        PAUSE, // 暫停
        INPROGRESS, // 進行中
        GAMEOVER // 遊戲結束
    };

    UI::UI() : score(8), heart(500, 9), MaxTime(90)
    {
    }

    void UI::LoadVolume() // 讀取聲音狀態
    {
        waveOutGetVolume(0, &volume);

        if (volume == 0)
        {
            volume = 0xFFFFFFFF;
            is_muted = true;
        }
        else
            is_muted = false;
    }

    void UI::LoadBitmap()
    {
        heart.LoadBitmap();
        char text[100] = { 0 };
        strcpy(text, "RES/UI/heartPointBoard.bmp");
        heartPointBoard.LoadBitmap(text);
        strcpy(text, "RES/UI/scoreBoard.bmp");
        scoreBoard.LoadBitmap(text);
        strcpy(text, "RES/UI/clock_background.bmp");
        clock_background.LoadBitmap(text, RGB(0, 0, 0));
        strcpy(text, "RES/UI/clock.bmp");
        clock.LoadBitmap(text, RGB(255, 255, 255));
        score.LoadBitmap();
        clock_center.SetPoint(heartPointBoard.Width() + (clock.Width() / 2) + 1, clock.Height() / 2 + 3);
        HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, "RES/UI/pointer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        //Add hbitmap to pointer CBitmap
        pointer.Attach(hbitmap);
        //Get pointer size
        pointer.GetBitmap(&pointer_size);
        audio_button_on.LoadBitmap("RES/UI/audio_button_on.bmp", RGB(0, 0, 0));
        audio_button_on_hovered.LoadBitmap("RES/UI/audio_button_on_hovered.bmp", RGB(0, 0, 0));
        audio_button_off.LoadBitmap("RES/UI/audio_button_off.bmp", RGB(0, 0, 0));
        audio_button_off_hovered.LoadBitmap("RES/UI/audio_button_off_hovered.bmp", RGB(0, 0, 0));
        up.LoadBitmap("RES/UI/up.bmp", RGB(255, 255, 255));
        up_hover.LoadBitmap("RES/UI/up_hover.bmp", RGB(255, 255, 255));
        down.LoadBitmap("RES/UI/down.bmp", RGB(255, 255, 255));
        down_hover.LoadBitmap("RES/UI/down_hover.bmp", RGB(255, 255, 255));
    }

    void UI::OnBeginState() // 初始化狀態
    {
        counter = 10;
        time_left = 90;
        is_reinforced = false;
        state = INPROGRESS;
        time_start.SetPoint(0, 0);
        time_end.SetPoint(0, 0);
        score.SetInteger(0);
        heart.SetPoint(2000);
        clock_radius = 30;
        angle = 0;
        xform.eDx = xform.eDy = xform.eM11 = xform.eM12 = xform.eM21 = xform.eM22 = 0;
        cursor_x = cursor_y = 0;
        rightButton = leftButton = false;
        is_muted = false;
        is_win = false;
    }

    void UI::OnMove()
    {
        if (state == INPROGRESS)
        {
            if (is_reinforced) // 特殊時間，用於扣除特殊時間量值
                heart.Add(-3);

            heart.OnMove(this);

            if (time_left >= 0) // 剩餘時間 >= 0，時鐘旋轉
            {
                angle = (float)((MaxTime - time_left) / 0.25);

                if (angle > 0)
                {
                    int cx = clock_center.x;
                    int cy = clock_center.y - clock_radius;
                    float distanceX = (float)(cos((-angle + 98) * 3.14159 / 180) * clock_center.x + sin((-angle + 98) * 3.14159 / 180) * clock_center.y);
                    float distanceY = (float)(cos((-angle + 98) * 3.14159 / 180) * clock_center.y - sin((-angle + 98) * 3.14159 / 180) * clock_center.x);
                    time_start.SetPoint(cx, cy);
                    time_end.SetPoint(int(clock_center.x + distanceX), int(clock_center.y + distanceY));
                    float radian = (float)(angle * 3.14159 / 180);
                    xform.eM11 = (float)cos(radian);
                    xform.eM12 = (float)sin(radian);
                    xform.eM21 = (float) - sin(radian);
                    xform.eM22 = (float)cos(radian);
                    xform.eDx = (float)(clock_center.x - cos(radian) * clock_center.x + sin(radian) * clock_center.y);
                    xform.eDy = (float)(clock_center.y - cos(radian) * clock_center.y - sin(radian) * clock_center.x);
                }
            }
            else // 時間到鐘聲響起，並GAMEOVER
            {
                CAudio::Instance()->Stop(AUDIO_GAME);
                CAudio::Instance()->Play(AUDIO_BELL, false);

                if (heart.GetPoint() > 0)
                    is_win = true;

                state = GAMEOVER;
            }

            if (counter == 29)
            {
                counter = 0;
                time_left--;
            }
            else
                counter++;
        }
    }

    void UI::OnShow(CGameMap* map)
    {
        heartPointBoard.SetTopLeft(0, 0);
        heartPointBoard.ShowBitmap();
        heart.SetTopLeft(20, heartPointBoard.Height() / 2);
        heart.OnShow(this, counter);
        clock_background.SetTopLeft(clock_center.x - clock_radius, clock_center.y - clock_radius);
        clock_background.ShowBitmap();

        if (angle > 0)
            DrawPie();

        clock.SetTopLeft(heartPointBoard.Width(), 0);
        clock.ShowBitmap();
        RotatePointer();
        scoreBoard.SetTopLeft(clock.Left() + clock.Width(), 0);
        scoreBoard.ShowBitmap();
        score.SetTopLeft(scoreBoard.Left() + 25, scoreBoard.Height() / 2);
        score.ShowBitmap();

        if (is_muted)
        {
            if (IsAudioButtonHoverd())
            {
                audio_button_off_hovered.SetTopLeft(scoreBoard.Left() + scoreBoard.Width() - audio_button_off_hovered.Width() - 10, 8);
                audio_button_off_hovered.ShowBitmap();
            }
            else
            {
                audio_button_off.SetTopLeft(scoreBoard.Left() + scoreBoard.Width() - audio_button_off.Width() - 10, 8);
                audio_button_off.ShowBitmap();
            }
        }
        else
        {
            if (IsAudioButtonHoverd())
            {
                audio_button_on_hovered.SetTopLeft(scoreBoard.Left() + scoreBoard.Width() - audio_button_on_hovered.Width() - 10, 8);
                audio_button_on_hovered.ShowBitmap();
            }
            else
            {
                audio_button_on.SetTopLeft(scoreBoard.Left() + scoreBoard.Width() - audio_button_on.Width() - 10, 8);
                audio_button_on.ShowBitmap();
            }
        }

        if (state == INPROGRESS)
        {
            if (rightButton || leftButton)
            {
                if (rightButton)
                {
                    up.SetTopLeft(680 - up.Width(), 150);
                    up_hover.SetTopLeft(680 - up_hover.Width(), 150);
                    down.SetTopLeft(692 - down.Width(), 300);
                    down_hover.SetTopLeft(692 - down_hover.Width(), 300);
                }
                else
                {
                    up.SetTopLeft(120, 150);
                    up_hover.SetTopLeft(120, 150);
                    down.SetTopLeft(130, 300);
                    down_hover.SetTopLeft(130, 300);
                }

                if (map->GetLevel() != 1)
                {
                    down.ShowBitmap();

                    if ((down.Left() <= cursor_x && cursor_x <= (down.Left() + down.Width())) && (down.Top() <= cursor_y && cursor_y <= (down.Top() + down.Height())))
                    {
                        down_hover.ShowBitmap();
                    }
                }

                if (map->GetLevel() != 4)
                {
                    up.ShowBitmap();

                    if ((up.Left() <= cursor_x && cursor_x <= up.Left() + up.Width()) && (up.Top() <= cursor_y && cursor_y <= up.Top() + up.Height()))
                    {
                        up_hover.ShowBitmap();
                    }
                }
            }
        }
    }

    void UI::OnMouseMove(CPoint point) // 處理滑鼠移動邏輯
    {
        cursor_x = point.x;
        cursor_y = point.y;
    }

    void UI::AddScore(int num) // 加分數
    {
        score.Add(num);
    }

    void UI::Toggle() // 靜音按鈕
    {
        is_muted = !is_muted;

        if (is_muted)
            waveOutSetVolume(0, 0);
        else
            waveOutSetVolume(0, volume);
    }

    void UI::Pause() // 暫停
    {
        state = PAUSE;
    }

    void UI::Resume() // 開始
    {
        state = INPROGRESS;
    }

    bool UI::IsAudioButtonHoverd() // 檢查滑鼠是否在按鈕上
    {
        if (is_muted)
            return (audio_button_off.Left() <= cursor_x && cursor_x <= audio_button_off.Left() + audio_button_off.Width()) && (audio_button_off.Top() <= cursor_y && cursor_y <= audio_button_off.Top() + audio_button_off.Height());
        else
            return (audio_button_on.Left() <= cursor_x && cursor_x <= audio_button_on.Left() + audio_button_on.Width()) && (audio_button_on.Top() <= cursor_y && cursor_y <= audio_button_on.Top() + audio_button_on.Height());
    }

    int UI::GetScore() // 取得當前分數
    {
        return score.GetInteger();
    }

    void UI::AddHeartPoints(int points) // 補充愛心
    {
        heart.Add(points);
    }

    void UI::SetHeartPoints(int points) // 設定愛心數量
    {
        heart.SetPoint(points);
    }

    int UI::GetHeartPoints() // 取得愛心
    {
        return heart.GetPoint();
    }

    void UI::GotoHRState(int state) // 愛心記分板進入state模式
    {
        heart.GotoHRState(state);

        if (state == CHeartPoint::reinforced)
            is_reinforced = true;
        else if (state == CHeartPoint::normal)
            is_reinforced = false;
    }

    bool UI::IsGameOver() // 是否遊戲結束
    {
        return state == GAMEOVER;
    }

    void UI::SetIsGameOver(bool status) // 設定遊戲結束
    {
        if (status)
        {
            is_win = heart.GetPoint() > 0;
            state = GAMEOVER;
        }
        else
            state = INPROGRESS;
    }

    void UI::SetIsButtonVisible(bool status, bool direction) // 設定是否顯示上下樓按鈕
    {
        if (direction)
            rightButton = status;
        else
            leftButton = status;
    }

    bool UI::IsUpButtonHoverd() // 上樓按鈕
    {
        return (rightButton || leftButton) && (up.Left() <= cursor_x && cursor_x <= up.Left() + up.Width()) && (up.Top() <= cursor_y && cursor_y <= up.Top() + up.Height());
    }

    bool UI::IsDownButtonHoverd() // 下樓按鈕
    {
        return (rightButton || leftButton) && (down.Left() <= cursor_x && cursor_x <= (down.Left() + down.Width())) && (down.Top() <= cursor_y && cursor_y <= (down.Top() + down.Height()));
    }

    bool UI::IsWin() // 贏的狀態下結束遊戲
    {
        return is_win;
    }

    void UI::DrawPie() // 畫時鐘背景
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CPen pen(PS_SOLID, 1, RGB(255, 214, 255));
        CBrush brush(RGB(255, 214, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Pie(clock_center.x - clock_radius, clock_center.y - clock_radius, clock_center.x + clock_radius, clock_center.y + clock_radius, time_end.x, time_end.y, time_start.x, time_start.y);
        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }

    void UI::RotatePointer() // 旋轉時鐘指針
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CDC ImageDC;
        ImageDC.CreateCompatibleDC(pDC);
        //ImageDC select bitmap
        CBitmap* bmOldPointer = ImageDC.SelectObject(&pointer);
        //Rotate image
        pDC->SetGraphicsMode(GM_ADVANCED);
        pDC->SetWorldTransform(&xform);
        //Make transparent with white color and paste to pDC
        pDC->TransparentBlt(clock_center.x - pointer_size.bmWidth / 2 - 2, clock_center.y - pointer_size.bmHeight + 2, pointer_size.bmWidth, pointer_size.bmHeight, &ImageDC, 0, 0, pointer_size.bmWidth, pointer_size.bmHeight, RGB(255, 255, 255));
        ImageDC.SelectObject(bmOldPointer);
        ImageDC.DeleteDC();
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}