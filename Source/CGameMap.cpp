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
    CGameMap::CGameMap() : sx(300), sy(0), level(1), width(2894), height(600)
    {
    }

    void CGameMap::LoadBitMap()
    {
        firstFloor.LoadBitmap(IDB_FIRSTFLOOR);
        secondFloor.LoadBitmap(IDB_SECONDFLOOR);
        thirdFloor.LoadBitmap(IDB_THIRDFLOOR);
        ceiling.LoadBitmap(IDB_CEILING);
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
        else
        {
            ceiling.SetTopLeft(-sx, -sy);
            ceiling.ShowBitmap();
        }
    }

    void CGameMap::Addsx(int value)
    {
        if (value > 0)
        {
            if (width - (sx + SIZE_X) > 0)
                sx += value;
        }
        else
        {
            if (sx > 0)
                sx += value;
        }
    }

    void CGameMap::Addsy(int value)
    {
        sy += value;
    }

    int CGameMap::Height()
    {
        return height;
    }

    int CGameMap::Width()
    {
        return width;
    }

    int CGameMap::ScreenX(int val)
    {
        return val - sx;
    }

    int CGameMap::ScreenY(int val)
    {
        return val - sy;
    }

    bool CGameMap::IsInScreen(int start_x, int end_x)
    {
        if (sx <= start_x && end_x <= sx + SIZE_X)
            return true;
        else
            return false;
    }

    int CGameMap::GetLevel()
    {
        return level;
    }

    void CGameMap::SetLevel(int level)
    {
        this->level = level;
    }
}