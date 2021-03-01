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

    void CGameMap::OnBeginState() // 初始化狀態
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
        // 延遲轉場時間
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

    void CGameMap::Addsx(int value) // 移動地圖X位置
    {
        if (value > 0)
        {
            if (width - (sx + SIZE_X) > 0) // 防止超出地圖外
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

    void CGameMap::Addsy(int value) // 移動地圖Y位置
    {
        sy += value;
    }

    int CGameMap::Height() // 取得高度
    {
        return height;
    }

    int CGameMap::Width() // 取得寬度
    {
        return width;
    }

    int CGameMap::ScreenX(int val) // 轉換地圖X座標至視窗X座標
    {
        return val - sx;
    }

    int CGameMap::ScreenY(int val) // 轉換地圖Y座標至視窗Y座標
    {
        return val - sy;
    }

    bool CGameMap::IsMapChanging() // 是否地圖正在切換
    {
        return is_mapChanging;
    }

    bool CGameMap::IsInScreen(int start_x, int end_x) // 是否在視窗範圍裡
    {
        return sx <= start_x && end_x <= sx + SIZE_X;
    }

    bool CGameMap::IsEmpty(int x, int y) // 是否該座標為空
    {
        return left_edge <= x && x <= right_edge;
    }

    int CGameMap::GetLevel() // 取得樓層
    {
        return level;
    }

    void CGameMap::SetLevel(int level) // 設定樓層
    {
        this->level = level;
        is_mapChanging = true;
    }
}