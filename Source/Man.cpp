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
	Man2::Man2(int x1, int y1, int start, int end, bool direction) : Man(1200, 150, 1200, 1950, direction) {}
	Man3::Man3(int x2, int y2, int start, int end, bool direction) : Man(1500, 135, 1500, 2060, direction) {}
	Man4::Man4(int x3, int y3, int start, int end, bool direction) : Man(400, 140, 400, 900, direction) {}
	Man5::Man5(int x4, int y4, int start, int end, bool direction) : Man(700, 140, 700, 1400, direction) {}


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

			if (direction) {
				normalMan1_right.OnMove();
			}
			else {
				normalMan1_left.OnMove();

			}

		}

	}

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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Man3::LoadBitMap()
	{
		normalMan3_left_stand.LoadBitmap("RES/normalMan3/left/stand.bmp", RGB(255, 255, 255));
		normalMan3_right_stand.LoadBitmap("RES/normalMan3/right/stand.bmp", RGB(255, 255, 255));


		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan3/right/normal_man3(" + to_string(i) + ").bmp").c_str());
			normalMan3_right.AddBitmap(text, RGB(255, 255, 255));
		}

		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan3/left/normal_man3(" + to_string(i) + ").bmp").c_str());
			normalMan3_left.AddBitmap(text, RGB(255, 255, 255));
		}
	}

	void Man3::OnMove(CGameMap* map)
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

			if (direction) {
				normalMan3_right.OnMove();
			}
			else {
				normalMan3_left.OnMove();
			}

		}
	}

	void Man3::OnShow(CGameMap* map)
	{

		if (moving) //是否正在移動
			if (direction) //false => 往左, true => 往右
			{
				normalMan3_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan3_right.OnShow();
			}
			else
			{

				normalMan3_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan3_left.OnShow();
			}
		else
		{
			if (direction) //false => 往左, true => 往右
			{
				normalMan3_right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan3_right_stand.ShowBitmap();

			}
			else
			{

				normalMan3_left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan3_left_stand.ShowBitmap();

			}
		}
	}

	void Man3::SetDirection(bool direction)
	{
		this->direction = direction;
	}

	void Man3::SetMoving(bool status)
	{
		moving = status;
	}


    //////////////////////////////////////////////////////////////////////////////////////////////

	void Man5::LoadBitMap()
	{
		normalMan2_left_stand.LoadBitmap("RES/normalMan2/left/stand.bmp", RGB(255, 255, 255));
		normalMan2_right_stand.LoadBitmap("RES/normalMan2/right/stand.bmp", RGB(255, 255, 255));


		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan2/right/normal_man2(" + to_string(i) + ").bmp").c_str());
			normalMan2_right.AddBitmap(text, RGB(255, 255, 255));
		}

		for (int i = 1; i <= 5; i++)
		{
			char text[100] = { 0 };
			strcpy(text, ("RES/normalMan2/left/normal_man2(" + to_string(i) + ").bmp").c_str());
			normalMan2_left.AddBitmap(text, RGB(255, 255, 255));
		}
	}

	void Man5::OnMove(CGameMap* map)
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

			if (direction) {
				normalMan2_right.OnMove();
			}
			else {
				normalMan2_left.OnMove();

			}

		}

	}

	void Man5::OnShow(CGameMap* map)
	{

		if (moving) //是否正在移動
			if (direction) //false => 往左, true => 往右
			{
				normalMan2_right.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan2_right.OnShow();

			}
			else
			{
				normalMan2_left.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan2_left.OnShow();
			}
		else
		{
			if (direction) //false => 往左, true => 往右
			{
				normalMan2_right_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan2_right_stand.ShowBitmap();

			}
			else
			{
				normalMan2_left_stand.SetTopLeft(map->ScreenX(x), map->ScreenY(y));
				normalMan2_left_stand.ShowBitmap();

			}
		}
	}


	void Man5::SetDirection(bool direction)
	{
		this->direction = direction;
	}

	void Man5::SetMoving(bool status)
	{
		moving = status;
	}
}
