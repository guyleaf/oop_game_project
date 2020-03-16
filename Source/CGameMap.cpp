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
    CGameMap::CGameMap() : sx(0), sy(0)
    {
        /*ParseCsv("floor.csv", map[0]);

        for (int i = 0; i < 17; i++)
        {
            for (int j = 0; j < 24; j++)
                cout << map[0][i][j] + ",";

            cout << endl;
        }*/
    }

    void CGameMap::LoadBitMap()
    {
        firstFloor.LoadBitmap(IDB_FIRSTFLOOR);
        secondFloor.LoadBitmap(IDB_SECONDFLOOR);
        ceiling.LoadBitmap(IDB_CEILING);
    }

    void CGameMap::OnShow(int level = 1)
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
        sx += value;
    }

    void CGameMap::Addsy(int value)
    {
        sy += value;
    }

    void CGameMap::ParseCsv(std::string name, int array[][24])
    {
        fstream data;
        data.open(name);
        string test;
        getline(data, test);

        if (data.is_open())
        {
            for (int i = 0; i < 17; i++)
            {
                for (int j = 0; j < 24; j++)
                {
                    string p;

                    if (j != 23)
                        getline(data, p, ',');
                    else
                        getline(data, p, '\n');

                    array[i][j] = stoi(p);
                }
            }
        }
        else
            ASSERT(0);
    }
}