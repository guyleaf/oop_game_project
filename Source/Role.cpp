#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "GameObject.h"

namespace game_framework
{
    int Role::GetX() // ���o�k��X�y�� (�a�Ϯy��)
    {
        return x;
    }

    int Role::GetY() // ���o�k��Y�y�� (�a�Ϯy��)
    {
        return y;
    }

    int Role::GetWidth() // ���o�k�ͼe��
    {
        return left_stand.Width();
    }

    int Role::GetHeight() // ���o�k�Ͱ���
    {
        return left_stand.Height();
    }
}