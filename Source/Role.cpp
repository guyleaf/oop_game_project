#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    int Role::GetX() // 取得男生X座標 (地圖座標)
    {
        return x;
    }

    int Role::GetY() // 取得男生Y座標 (地圖座標)
    {
        return y;
    }

    int Role::GetWidth() // 取得男生寬度
    {
        return left_stand.Width();
    }

    int Role::GetHeight() // 取得男生高度
    {
        return left_stand.Height();
    }
}