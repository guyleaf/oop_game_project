#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    enum State // ���A
    {
        ALIVE, // �s��
        ATTACKING, // ����
        FLYING, // ����
        LEAVING, // ���}
        DEAD // ���`
    };

    Girl::Girl()
    {
    }

    Girl::~Girl()
    {
    }

    void Girl::SetDirection(bool direction) // �]�w���ʤ�V
    {
        this->direction = direction;
    }

    void Girl::OnMove(CGameMap* map, int seed)
    {
        if (status == ALIVE) // �s��
        {
            if (is_shocking) // �k�ͪ`�N��k�D�����b�y�b�k��
            {
                exclamation.OnMove();

                if (direction)
                    surprising_right.OnMove();
                else
                    surprising_left.OnMove();
            }

            moving = seed % 2; // �H������

            if (moving && !is_shocking) // ���b����and�S���m�ܨƥ�
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
                    right.OnMove();
                else
                    left.OnMove();
            }
        }
        else if (status == FLYING) // ����
        {
            if (map->IsInScreen(x, x + left_stand.Width()))
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
        else if (status == LEAVING) // ���}
        {
            if (distance > 0)
            {
                if (direction)
                {
                    x += 5;
                    leaving_right.OnMove();
                }
                else
                {
                    x -= 5;
                    leaving_left.OnMove();
                }

                fun.OnMove();
                distance -= 5;
            }
            else
                status = DEAD;
        }
    }

    void Girl::OnShow(CGameMap* map)
    {
        if (status == ALIVE) // �s��
        {
            if (is_shocking) // �k�ͪ`�N��k�D�����b�y�b�k��
            {
                exclamation.SetTopLeft(map->ScreenX(x) + left_stand.Width() / 2, map->ScreenY(y) - exclamation.Height());
                exclamation.OnShow();

                if (direction)
                {
                    beam_pos[2].SetPoint(map->ScreenX(x) + left_stand.Width() / 2 + 15, map->ScreenY(y) + left_stand.Height() / 5 - 10);
                    beam_pos[3].SetPoint(map->ScreenX(x) + left_stand.Width() / 2 + 5, map->ScreenY(y) + left_stand.Height() / 5 + 10);
                    notice_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                    notice_right.ShowBitmap();
                    surprising_right.SetTopLeft(map->ScreenX(x + left_stand.Width()), map->ScreenY(y));
                    surprising_right.OnShow();
                }
                else
                {
                    beam_pos[3].SetPoint(map->ScreenX(x) + left_stand.Width() / 2 - 20, map->ScreenY(y) + left_stand.Height() / 5 - 10);
                    beam_pos[2].SetPoint(map->ScreenX(x) + left_stand.Width() / 2 - 5, map->ScreenY(y) + left_stand.Height() / 5 + 10);
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

                if (moving) //�O�_���b����
                    if (direction) //false => ����, true => ���k
                    {
                        right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        right.OnShow();
                    }
                    else
                    {
                        left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        left.OnShow();
                    }
                else
                {
                    if (direction) //false => ����, true => ���k
                    {
                        right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        right_stand.ShowBitmap();
                    }
                    else
                    {
                        left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                        left_stand.ShowBitmap();
                    }
                }
            }
        }
        else if (status == ATTACKING) // ����
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

            status = ALIVE; // �C�����@���ʵe���@��
        }
        else if (status == FLYING) // ����
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
        else if (status == LEAVING) // ���}
        {
            if (direction)
            {
                leaving_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                leaving_right.OnShow();
            }
            else
            {
                leaving_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
                leaving_left.OnShow();
            }

            fun.SetTopLeft(map->ScreenX(x) + 12, map->ScreenY(y));
            fun.OnShow();
        }
    }

    void Girl::SetIsShocking(bool status) // �]�w�O�_�`�N��k�D�����b�y�b�k��
    {
        is_shocking = status;
    }

    bool Girl::IsShocking() // �O�_�`�N��k�D�����b�y�b�k��
    {
        return is_shocking;
    }

    void Girl::LockPerson(Man* man, CGameMap* map) // ��w�k��
    {
        beam_pos[0].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 - 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
        beam_pos[1].SetPoint(map->ScreenX(man->GetX()) + man->GetWidth() / 2 + 10, map->ScreenY(man->GetY()) + man->GetHeight() / 3);
    }

    void Girl::Attack(Man* man, CGameMap* map) // �����k��
    {
        man->LoseHP(-damage);
        status = ATTACKING;
    }


    bool Girl::IsLocked() // �O�_��w
    {
        return exclamation.IsFinalBitmap();
    }

    void Girl::Win() // Ĺ
    {
        status = LEAVING;
    }

    void Girl::Lose() // ��
    {
        status = FLYING;
        CAudio::Instance()->Play(AUDIO_FLYING, false);
    }

    bool Girl::IsAlive() // �O�_�s��
    {
        return status == ALIVE;
    }

    bool Girl::IsAlreadyDead() // �O�_�w�g���z
    {
        return status == DEAD;
    }

    void Girl::DrawBeam(CGameMap* map) // �e�X�p�g����
    {
        CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
        CPen pen(PS_SOLID, 3, RGB(255, 166, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush brush(RGB(255, 209, 0));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Polygon(beam_pos, 4);
        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
        CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
    }
    ////////////////////////////////////////////////////////////////////////////////////

    NormalGirl::NormalGirl(int x, int y, int start, int end, bool direction, int type) : Girl(), x(x), y(y), start(start), end(end), direction(direction), type(type)
    {
    }

    NormalGirl::~NormalGirl()
    {
    }

    void NormalGirl::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/stand.bmp").c_str());
        left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/stand.bmp").c_str());
        right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/normalGirl (" + to_string(i) + ").bmp").c_str());
            right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/normalGirl (" + to_string(i) + ").bmp").c_str());
            left.AddBitmap(text, RGB(255, 255, 255));
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

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/left/win (" + to_string(i) + ").bmp").c_str());
            leaving_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/Girl/normalGirl" + to_string(type) + "/right/win (" + to_string(i) + ").bmp").c_str());
            leaving_right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/Girl/fun (" + to_string(i) + ").bmp").c_str());
            fun.AddBitmap(text, RGB(255, 255, 255));
        }

        fun.SetDelayCount(5);
    }

    void NormalGirl::OnBeginState() // ��l�ƪ��A
    {
        Girl::damage = 3;
        Girl::x = x;
        Girl::y = y;
        Girl::moving = true;
        Girl::direction = direction;
        Girl::velocity = 2;
        range[0] = start;
        range[1] = end;
        is_shocking = false;
        status = ALIVE;
        distance = 500;
        exclamation.Reset();
        flying_left.Reset();
        flying_right.Reset();
    }

    bool NormalGirl::IsSpecialGirl() // �O�_�����v�S��k��
    {
        return false;
    }

    SpecialGirl::SpecialGirl(int x, int y, int start, int end, bool direction, int type) : Girl(), x(x), y(y), start(start), end(end), direction(direction), type(type)
    {
    }

    SpecialGirl::~SpecialGirl()
    {
    }

    void SpecialGirl::LoadBitMap()
    {
        char text[100] = { 0 };
        strcpy(text, "RES/Girl/specialGirl/left/stand.bmp");
        left_stand.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/specialGirl/right/stand.bmp");
        right_stand.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/specialGirl/right/specialGirl (" + to_string(i) + ").bmp").c_str());
            right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/specialGirl/left/specialGirl (" + to_string(i) + ").bmp").c_str());
            left.AddBitmap(text, RGB(255, 255, 255));
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
            strcpy(text, ("RES/Girl/specialGirl/left/flying (" + to_string(i) + ").bmp").c_str());
            flying_left.AddBitmap(text, RGB(255, 255, 255));
        }

        flying_left.SetDelayCount(1);

        for (int i = 1; i <= 4; i++)
        {
            strcpy(text, ("RES/Girl/specialGirl/right/flying (" + to_string(i) + ").bmp").c_str());
            flying_right.AddBitmap(text, RGB(255, 255, 255));
        }

        flying_right.SetDelayCount(1);
        strcpy(text, "RES/Girl/specialGirl/left/shooting.bmp");
        shooting_left.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/specialGirl/right/shooting.bmp");
        shooting_right.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/specialGirl/left/notice.bmp");
        notice_left.LoadBitmap(text, RGB(255, 255, 255));
        strcpy(text, "RES/Girl/specialGirl/right/notice.bmp");
        notice_right.LoadBitmap(text, RGB(255, 255, 255));

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/specialGirl/left/win (" + to_string(i) + ").bmp").c_str());
            leaving_left.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 5; i++)
        {
            strcpy(text, ("RES/Girl/specialGirl/right/win (" + to_string(i) + ").bmp").c_str());
            leaving_right.AddBitmap(text, RGB(255, 255, 255));
        }

        for (int i = 1; i <= 7; i++)
        {
            strcpy(text, ("RES/Girl/fun (" + to_string(i) + ").bmp").c_str());
            fun.AddBitmap(text, RGB(255, 255, 255));
        }

        fun.SetDelayCount(5);
    }

    void SpecialGirl::OnBeginState() // ��l�ƪ��A
    {
        Girl::damage = 18;
        Girl::x = x;
        Girl::y = y;
        Girl::moving = true;
        Girl::direction = direction;
        Girl::velocity = 2;
        range[0] = start;
        range[1] = end;
        is_shocking = false;
        status = ALIVE;
        distance = 500;
        exclamation.Reset();
        flying_left.Reset();
        flying_right.Reset();
    }

    bool SpecialGirl::IsSpecialGirl() // �O�_�����v�S��k��
    {
        return true;
    }
}