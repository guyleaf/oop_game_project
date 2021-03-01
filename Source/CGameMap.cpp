#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace game_framework
{
    CGameMap::CGameMap()
    {
    }

    void CGameMap::LoadBitMap()
    {
        firstFloor.LoadBitmap(IDB_FIRSTFLOOR);
        secondFloor.LoadBitmap(IDB_SECONDFLOOR);
        thirdFloor.LoadBitmap(IDB_THIRDFLOOR);
        ceiling.LoadBitmap(IDB_CEILING);
    }

    void CGameMap::OnBeginState() // ��l�ƪ��A
    {
        sx = 300;
        sy = 0;
        level = 1;
        width = 2894;
        height = 600;
        left_edge = 236;
        right_edge = 2688;
        counter = 25;
        is_mapChanging = false;
    }

    void CGameMap::OnMove()
    {
        // ��������ɶ�
        if (counter == 0)
        {
            counter = 25;
            is_mapChanging = false;
        }

        if (is_mapChanging)
        {
            counter--;
        }
    }

    void CGameMap::OnShow()
    {
        if (level == 1)
        {
            firstFloor.SetTopLeft(-sx, -sy);
            firstFloor.ShowBitmap();
        }
        else if (level == 2)
        {
            secondFloor.SetTopLeft(-sx, -sy);
            secondFloor.ShowBitmap();
        }
        else if (level == 3)
        {
            thirdFloor.SetTopLeft(-sx, -sy);
            thirdFloor.ShowBitmap();
        }
        else
        {
            ceiling.SetTopLeft(-sx, -sy);
            ceiling.ShowBitmap();
        }
    }

    void CGameMap::Addsx(int value) // ���ʦa��X��m
    {
        if (value > 0)
        {
            if (width - (sx + SIZE_X) > 0) // ����W�X�a�ϥ~
                sx += value;
            else
                sx = width - SIZE_X;
        }
        else
        {
            if (sx > 0)
                sx += value;
            else
                sx = 0;
        }
    }

    void CGameMap::Addsy(int value) // ���ʦa��Y��m
    {
        sy += value;
    }

    int CGameMap::Height() // ���o����
    {
        return height;
    }

    int CGameMap::Width() // ���o�e��
    {
        return width;
    }

    int CGameMap::ScreenX(int val) // �ഫ�a��X�y�Цܵ���X�y��
    {
        return val - sx;
    }

    int CGameMap::ScreenY(int val) // �ഫ�a��Y�y�Цܵ���Y�y��
    {
        return val - sy;
    }

    bool CGameMap::IsMapChanging() // �O�_�a�ϥ��b����
    {
        return is_mapChanging;
    }

    bool CGameMap::IsInScreen(int start_x, int end_x) // �O�_�b�����d���
    {
        return sx <= start_x && end_x <= sx + SIZE_X;
    }

    bool CGameMap::IsEmpty(int x, int y) // �O�_�Ӯy�Ь���
    {
        return left_edge <= x && x <= right_edge;
    }

    int CGameMap::GetLevel() // ���o�Ӽh
    {
        return level;
    }

    void CGameMap::SetLevel(int level) // �]�w�Ӽh
    {
        this->level = level;
        is_mapChanging = true;
    }
}