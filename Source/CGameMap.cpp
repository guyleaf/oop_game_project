#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace game_framework
{
    CGameMap::CGameMap() : sx(300), sy(0), level(1)
    {
    }

    void CGameMap::LoadBitMap()
    {
        firstFloor.LoadBitmap(IDB_FIRSTFLOOR);
        secondFloor.LoadBitmap(IDB_SECONDFLOOR);
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
            if (MAP_W - (sx + SIZE_X) > 0)
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

    int CGameMap::ScreenX(int val)
    {
        return val - sx;
    }

    int CGameMap::ScreenY(int val)
    {
        return val - sy;
    }

    bool CGameMap::IsEmpty(int x, int y)
    {
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