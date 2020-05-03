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
        INPROGRESS,
        GAMEOVER
    };

    UI::UI() : score(8), counter(10), time_left(89), is_reinforced(false), heartPoints(0), state(INPROGRESS), MaxTime(90)
    {
        time_start.SetPoint(0, 0);
        time_end.SetPoint(0, 0);
        score.SetInteger(0);
        clock_radius = 30;
        angle = 0;
    }

    void UI::LoadBitmap()
    {
        char text[100] = { 0 };
        strcpy(text, "RES/UI/scoreBoard.bmp");
        scoreBoard.LoadBitmap(text);
        strcpy(text, "RES/UI/clock_background.bmp");
        clock_background.LoadBitmap(text, RGB(0, 0, 0));
        strcpy(text, "RES/UI/clock.bmp");
        clock.LoadBitmap(text, RGB(255, 255, 255));
        score.LoadBitmap();
        clock_center.SetPoint(clock.Left() + clock.Width() / 2 + 1, clock.Top() + clock.Height() / 2 + 3);
        HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, "RES/UI/pointer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        //Add hbitmap to pointer CBitmap
        pointer.Attach(hbitmap);
        //Get pointer size
        pointer.GetBitmap(&pointer_size);
    }

    void UI::OnMove()
    {
        if (state == INPROGRESS)
        {
            if (is_reinforced)
            {
                //Special mode
            }
            else
            {
                //heartPoint board
            }

            if (time_left >= 0)
            {
                angle = (float)((MaxTime - time_left) / 0.25);

                if (angle > 0)
                {
                    int cx = clock_center.x;
                    int cy = clock_center.y - clock_radius;
                    float distanceX = (float)(cos((-angle + 132.3) * 3.14159 / 180) * clock_center.x + sin((-angle + 132.3) * 3.14159 / 180) * clock_center.y);
                    float distanceY = (float)(cos((-angle + 132.3) * 3.14159 / 180) * clock_center.y - sin((-angle + 132.3) * 3.14159 / 180) * clock_center.x);
                    time_start.SetPoint(cx, cy);
                    time_end.SetPoint(int(clock_center.x + distanceX), int(clock_center.x + distanceY));
                }
            }
            else
            {
                CAudio::Instance()->Stop(AUDIO_GAME);
                CAudio::Instance()->Play(AUDIO_BELL, false);
                state = GAMEOVER;
            }
        }
        else if (state == GAMEOVER)
        {
        }

        if (counter == 29)
        {
            counter = 0;
            time_left--;
        }
        else
            counter++;
    }

    void UI::OnShow()
    {
        scoreBoard.SetTopLeft(clock.Width(), 0);
        scoreBoard.ShowBitmap();
        score.SetTopLeft(scoreBoard.Left() + 25, scoreBoard.Height() / 2);
        score.ShowBitmap();
        clock_background.SetTopLeft(clock_center.x - clock_radius, clock_center.y - clock_radius);
        clock_background.ShowBitmap();

        if (angle > 0)
            DrawPie();

        clock.SetTopLeft(0, 0);
        clock.ShowBitmap();
        RotatePointer();
    }

    void UI::AddScore(int num)
    {
        score.Add(num);
    }

    int UI::GetScore()
    {
        return score.GetInteger();
    }

    void UI::SetHeartPoints(int points)
    {
        heartPoints = points;
    }

    void UI::SetIsReinforced(bool status)
    {
        is_reinforced = status;
    }

    bool UI::IsReinforced()
    {
        return is_reinforced;
    }

    bool UI::IsGameOver()
    {
        return state == GAMEOVER;
    }

    void UI::DrawPie()
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

    void UI::RotatePointer()
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CDC ImageDC;
        ImageDC.CreateCompatibleDC(pDC);
        //ImageDC select bitmap
        CBitmap* bmOldPointer = ImageDC.SelectObject(&pointer);
        //Rotate image
        float radian = (float)(angle * (3.14159 / 180));
        XFORM xform;
        xform.eM11 = (float)cos(radian);
        xform.eM12 = (float)sin(radian);
        xform.eM21 = (float) - sin(radian);
        xform.eM22 = (float)cos(radian);
        xform.eDx = (float)(clock_center.x - cos(radian) * clock_center.x + sin(radian) * clock_center.y);
        xform.eDy = (float)(clock_center.y - cos(radian) * clock_center.y - sin(radian) * clock_center.x);
        pDC->SetGraphicsMode(GM_ADVANCED);
        pDC->SetWorldTransform(&xform);
        //Make transparent with white color and paste to pDC
        pDC->TransparentBlt(clock_center.x - pointer_size.bmWidth / 2 - 2, clock_center.y - pointer_size.bmHeight + 2, pointer_size.bmWidth, pointer_size.bmHeight, &ImageDC, 0, 0, pointer_size.bmWidth, pointer_size.bmHeight, RGB(255, 255, 255));
        ImageDC.SelectObject(bmOldPointer);
        ImageDC.DeleteDC();
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
