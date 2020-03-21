#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "Man.h"

namespace game_framework
{
	Man::Man(int x, int y, int start, int end, bool direction) : x(x), y(y), moving(true), direction(direction), velocity(5)
	{
		range[0] = start;
		range[1] = end;
	}

	void Man::LoadBitMap()
	{
		normalMan1_left_stand.LoadBitmap("RES/normalMan1/left/stand.bmp", RGB(255, 255, 255));
		normalMan1_right_stand.LoadBitmap("RES/normalMan1/right/stand.bmp", RGB(255, 255, 255));

		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan1/right/normal_man1(" + to_string(i) + ").bmp").c_str());
			normalMan1_right.AddBitmap(text, RGB(255, 255, 255));
		}

		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan1/left/normal_man1(" + to_string(i) + ").bmp").c_str());
			normalMan1_left.AddBitmap(text, RGB(255, 255, 255));
		}
	}

	void Man::OnMove(CGameMap* map)
	{
		if (moving)
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
				normalMan1_right.OnMove();
			else
				normalMan1_left.OnMove();
		}

	}



/*	void Man::SetMoving(CGameMap* map, CPoint point)
	{
		if (point.x - (map->ScreenX(x) + width) > 0) //滑鼠座標與人物最右邊的座標相減(螢幕的點座標) 需大於0
		{
			moving = true;
			direction = true;
		}
		else if (map->ScreenX(x) - point.x > 0)
		{
			moving = true;
			direction = false;
		}
		else
			moving = false;
	}*/

	void Man::OnShow(CGameMap* map)
	{

		if (moving) //是否正在移動
			if (direction) //false => 往左, true => 往右
			{
				normalMan1_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan1_right.OnShow();
			}
			else
			{
				normalMan1_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan1_left.OnShow();
			}
		else
		{
			if (direction) //false => 往左, true => 往右
			{
				normalMan1_right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan1_right_stand.ShowBitmap();
			}
			else
			{
				normalMan1_left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan1_left_stand.ShowBitmap();
			}
		}
	}


	void Man::SetDirection(bool direction)
	{
		this->direction = direction;
	}

	void Man::SetMoving(bool status)
	{
		moving = status;
	}
}