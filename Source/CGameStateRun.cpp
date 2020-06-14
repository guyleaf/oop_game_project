#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateRun.h"
#include <regex>

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
    /////////////////////////////////////////////////////////////////////////////

    CGameStateRun::CGameStateRun(CGame* g, int* score) : CGameState(g, score)
    {
        mainGirl = new MainGirl();
        LoadData();
        teacher = NULL;
        isGoldBoyGenerated = false;
    }

    CGameStateRun::~CGameStateRun()
    {
        for (int j = 0; j < 4; j++)
        {
            for (size_t i = 0; i < man[j][0].size(); i++)
            {
                delete man[j][0][i];
            }

            for (size_t i = 0; i < man[j][1].size(); i++)
            {
                delete man[j][1][i];
            }

            for (size_t i = 0; i < girl[j][0].size(); i++)
            {
                delete girl[j][0][i];
            }

            for (size_t i = 0; i < girl[j][1].size(); i++)
            {
                delete girl[j][1][i];
            }
        }

        for (size_t i = 0; i < hearts.size(); i++)
        {
            delete hearts[i];
        }

        delete mainGirl;
        delete teacher;
    }

    void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
        //
        // 開始載入資料
        //
        map.LoadBitMap();
        mainGirl->LoadBitMap();

        for (int j = 0; j < 4; j++)
        {
            for (size_t i = 0; i < man[j][0].size(); i++)
            {
                man[j][0][i]->LoadBitMap();
            }

            for (size_t i = 0; i < man[j][1].size(); i++)
            {
                man[j][1][i]->LoadBitMap();
            }

            for (size_t i = 0; i < girl[j][0].size(); i++)
            {
                girl[j][0][i]->LoadBitMap();
            }

            for (size_t i = 0; i < girl[j][1].size(); i++)
            {
                girl[j][1][i]->LoadBitMap();
            }
        }

        ui.LoadBitmap();
        //
        // 完成部分Loading動作，提高進度
        //
        ShowInitProgress(50);
        //Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //CAudio::Instance()->Load(AUDIO_GAME, "sounds\\game.mp3");	// 載入編號0的聲音game.mp3
        CAudio::Instance()->Load(AUDIO_LASER, "sounds\\laser.mp3");
        CAudio::Instance()->Load(AUDIO_EAT_HEART, "sounds\\eatheart.mp3");
        CAudio::Instance()->Load(AUDIO_FLYING, "sounds\\flying.mp3");
        CAudio::Instance()->Load(AUDIO_BELL, "sounds\\bell.mp3");
        CAudio::Instance()->Load(AUDIO_SNATCH, "sounds\\snatch.mp3");
        CAudio::Instance()->Load(AUDIO_REINFORCING, "sounds\\reinforcing.mp3");
        CAudio::Instance()->Load(AUDIO_BLINK, "sounds\\blink.mp3");
        CAudio::Instance()->Load(AUDIO_BUMP, "sounds\\bump.mp3");
        CAudio::Instance()->Load(AUDIO_SUMMARIZE, "sounds\\summarize.mp3");
        CAudio::Instance()->Load(AUDIO_LOSE, "sounds\\lose.mp3");
        //
        // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
        //
    }

    void CGameStateRun::OnBeginState()
    {
        ui.OnBeginState();
        ui.LoadVolume();
        map.OnBeginState();
        mainGirl->OnBeginState();

        for (int level = 0; level < 4; level++)
        {
            int index = -1;

            for (size_t i = 0; i < man[level][0].size(); i++)
            {
                if (man[level][0][i]->GetScore() != 40000)
                    man[level][0][i]->OnBeginState();
                else
                    index = i;
            }

            if (index != -1)
            {
                delete man[level][0][index];
                man[level][0].erase(man[level][0].begin() + index);
            }

            index = -1;

            for (size_t i = 0; i < man[level][1].size(); i++)
            {
                if (man[level][1][i]->GetScore() != 40000)
                    man[level][1][i]->OnBeginState();
                else
                    index = i;
            }

            if (index != -1)
            {
                delete man[level][1][index];
                man[level][1].erase(man[level][1].begin() + index);
            }

            index = -1;

            for (size_t i = 0; i < girl[level][0].size(); i++)
            {
                if (!girl[level][0][i]->IsSpecialGirl())
                    girl[level][0][i]->OnBeginState();
                else
                    index = i;
            }

            if (index != -1)
            {
                delete girl[level][0][index];
                girl[level][0].erase(girl[level][0].begin() + index);
            }

            index = -1;

            for (size_t i = 0; i < girl[level][1].size(); i++)
            {
                if (!girl[level][1][i]->IsSpecialGirl())
                    girl[level][1][i]->OnBeginState();
                else
                    index = i;
            }

            if (index != -1)
            {
                delete girl[level][1][index];
                girl[level][1].erase(girl[level][1].begin() + index);
            }
        }

        srand((unsigned int)time(NULL));
        delete teacher;
        teacher = new Teacher(rand() % 4 + 1, 1000, MIDDLE);
        teacher->LoadBitmap();
        isGoldBoyGenerated = false;
    }

    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        //
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        //
        //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        //mainGirl->SetIsFocusing(false);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int level = map.GetLevel();
        map.OnMove();
        ui.OnMove();
        srand((unsigned)time(NULL));

        if (!isGoldBoyGenerated && !mainGirl->IsInAnimation() && mainGirl->IsReinforced())
        {
            GenerateGoldBoy(0, false, rand() % 2);
            isGoldBoyGenerated = true;
        }

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation() && mainGirl->IsReinforced())
        {
            ui.GotoHRState(CHeartPoint::reinforced);
            ui.Resume();
        }

        if (ui.IsGameOver())
        {
            if (mainGirl->IsAttacking())
                CAudio::Instance()->Stop(AUDIO_LASER);

            mainGirl->SetIsLocked(false);
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
        }

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation())
        {
            if (teacher->IsInLevel(level) && teacher->HitMainGirl(mainGirl))
            {
                if (mainGirl->IsAttacking())
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);

                if (!mainGirl->IsReinforced())
                    ui.AddHeartPoints(-600);

                CAudio::Instance()->Play(AUDIO_FLYING, false);
                mainGirl->Lose();
            }
            else if (mainGirl->IsAttacking() && !mainGirl->IsReinforced() && ui.GetHeartPoints() <= 0)
            {
                if (mainGirl->IsAttacking())
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);
                CAudio::Instance()->Play(AUDIO_FLYING, false);
                mainGirl->Lose();
            }
        }

        teacher->OnMove(&map);

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation())
        {
            for (size_t i = 0; i < man[level - 1][0].size(); i++)
            {
                if (man[level - 1][0][i]->IsAlive() && !mainGirl->IsFocusing() && man[level - 1][0][i]->HitMainGirl(&map, mainGirl))
                {
                    man[level - 1][0][i]->SetIsFocused(true);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[level - 1][0][i]);
                }
            }

            for (size_t i = 0; i < man[level - 1][1].size(); i++)
            {
                if (man[level - 1][1][i]->IsAlive() && !mainGirl->IsFocusing() && man[level - 1][1][i]->HitMainGirl(&map, mainGirl))
                {
                    man[level - 1][1][i]->SetIsFocused(true);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[level - 1][1][i]);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < man[level - 1][0].size(); i++)
            {
                if (man[level - 1][0][i]->IsAlive())
                {
                    man[level - 1][0][i]->SetIsFocused(false);
                }
            }

            for (size_t i = 0; i < man[level - 1][1].size(); i++)
            {
                if (man[level - 1][1][i]->IsAlive())
                {
                    man[level - 1][1][i]->SetIsFocused(false);
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int i = 0; i < 4; i++)
        {
            for (vector<Man*>::iterator person = man[i][0].begin(); person != man[i][0].end(); person++)
            {
                if (!(*person)->IsFollowing(Man::mainGirl))
                    (*person)->OnMove(rand());
            }

            for (vector<Man*>::iterator person = man[i][1].begin(); person != man[i][1].end(); person++)
            {
                if (!(*person)->IsFollowing(Man::mainGirl))
                    (*person)->OnMove(rand());
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (mainGirl->IsAttacking() && girlsOnScreen.size() == 0)
        {
            for (size_t i = 0; i < girl[level - 1][0].size(); i++)
            {
                if (map.IsInScreen(girl[level - 1][0][i]->GetX(), girl[level - 1][0][i]->GetX() + girl[level - 1][0][i]->GetWidth()) && girl[level - 1][0][i]->IsAlive())
                {
                    girlsOnScreen.push_back(girl[level - 1][0][i]);
                }
            }

            for (size_t i = 0; i < girl[level - 1][1].size(); i++)
            {
                if (map.IsInScreen(girl[level - 1][1][i]->GetX(), girl[level - 1][1][i]->GetX() + girl[level - 1][1][i]->GetWidth()) && girl[level - 1][1][i]->IsAlive())
                {
                    girlsOnScreen.push_back(girl[level - 1][1][i]);
                }
            }

            if (girlsOnScreen.size() == 0)
            {
                girlsOnScreen.push_back(NULL);
            }
        }
        else if (!mainGirl->IsAttacking())
        {
            for (size_t i = 0; i < girlsOnScreen.size(); i++)
            {
                if (girlsOnScreen[i] == NULL)
                    break;

                girlsOnScreen[i]->SetIsShocking(false);
            }

            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < man[level - 1][0].size(); i++)
        {
            if (man[level - 1][0][i]->IsAlreadyDead() && !man[level - 1][0][i]->IsFollowing(Man::mainGirl) && !man[level - 1][0][i]->IsFollowing(Man::girl))
            {
                if (man[level - 1][0][i]->IsKilledBy(Man::mainGirl))
                {
                    mainGirl->AddSlave(man[level - 1][0][i]);
                    man[level - 1][0][i]->SetIsFollowing(Man::mainGirl);
                    break;
                }
                else
                {
                    man[level - 1][0][i]->SetIsFollowing(Man::girl);
                }
            }

            if (!ui.IsGameOver())
            {
                int HP = int(man[level - 1][0][i]->GetHP());

                if (mainGirl->IsLocked() && man[level - 1][0][i]->IsAttackedBy(Man::all) && man[level - 1][0][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (HP >= 800 || HP <= 0)
                    {
                        man[level - 1][0][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP <= 0)
                        {
                            int num = 0;

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                            {
                                if (girlsOnScreen[j]->IsSpecialGirl())
                                    num += 5;
                                else
                                    num += 1;
                            }

                            hearts.push_back(new Heart(0, 1, man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2, man[level - 1][0][i]->GetY(), num));

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Lose();

                            man[level - 1][0][i]->SetIsKilledBy(Man::mainGirl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }
                        else if (HP >= 800)
                        {
                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Win();

                            ui.AddHeartPoints(-750);
                            CAudio::Instance()->Play(AUDIO_FLYING, false);
                            mainGirl->Lose();
                            man[level - 1][0][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }

                    if (mainGirl->IsClicked())
                    {
                        mainGirl->Attack(man[level - 1][0][i], &map);

                        for (size_t j = 0; j < girlsOnScreen.size(); j++)
                        {
                            if (girlsOnScreen[j]->IsSpecialGirl())
                                ui.AddScore(15);
                            else
                                ui.AddScore(3);
                        }

                        if (!mainGirl->IsReinforced())
                            ui.AddHeartPoints(-18 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                    {
                        girlsOnScreen[j]->Attack(man[level - 1][0][i], &map);
                    }
                }
                else
                {
                    if (man[level - 1][0][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[level - 1][0][i]))
                    {
                        if (HP <= 0)
                        {
                            man[level - 1][0][i]->SetIsAlive(false);
                            man[level - 1][0][i]->SetIsKilledBy(Man::mainGirl);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            hearts.push_back(new Heart(0, 0, man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2, man[level - 1][0][i]->GetY(), 0));
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }

                        if (man[level - 1][0][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[level - 1][0][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[level - 1][0][i], &map);
                                ui.AddScore(1);
                                ui.AddHeartPoints(-8);

                                if (girlsOnScreen.size() != 0)
                                {
                                    int ready = 0;

                                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                    {
                                        if (girlsOnScreen[j] == NULL)
                                            break;

                                        if (girlsOnScreen[j]->IsLocked())
                                            ready++;

                                        if (girlsOnScreen[j]->GetX() >= man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2)
                                            girlsOnScreen[j]->SetDirection(false);
                                        else
                                            girlsOnScreen[j]->SetDirection(true);

                                        girlsOnScreen[j]->LockPerson(man[level - 1][0][i], &map);
                                        girlsOnScreen[j]->SetIsShocking(true);
                                    }

                                    if (ready == girlsOnScreen.size() && mainGirl->IsAttacking())
                                    {
                                        man[level - 1][0][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true);
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false);
                                    }
                                }
                            }
                        }
                        else
                        {
                            man[level - 1][0][i]->SetIsFocused(false);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < man[level - 1][1].size(); i++)
        {
            if (man[level - 1][1][i]->IsAlreadyDead() && !man[level - 1][1][i]->IsFollowing(Man::mainGirl) && !man[level - 1][1][i]->IsFollowing(Man::girl))
            {
                if (man[level - 1][1][i]->IsKilledBy(Man::mainGirl))
                {
                    mainGirl->AddSlave(man[level - 1][1][i]);
                    man[level - 1][1][i]->SetIsFollowing(Man::mainGirl);
                    break;
                }
                else
                {
                    man[level - 1][1][i]->SetIsFollowing(Man::girl);
                }
            }

            if (!ui.IsGameOver())
            {
                int HP = int(man[level - 1][1][i]->GetHP());

                if (man[level - 1][1][i]->IsAttackedBy(Man::all) && man[level - 1][1][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (HP >= 800 || HP <= 0)
                    {
                        man[level - 1][1][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP <= 0)
                        {
                            int num = 0;

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                            {
                                if (girlsOnScreen[j]->IsSpecialGirl())
                                    num += 5;
                                else
                                    num += 1;
                            }

                            hearts.push_back(new Heart(1, 1, man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2, man[level - 1][1][i]->GetY() - 55, num));

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Lose();

                            man[level - 1][1][i]->SetIsKilledBy(Man::mainGirl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }
                        else if (HP >= 800)
                        {
                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Win();

                            ui.AddHeartPoints(-750);
                            CAudio::Instance()->Play(AUDIO_FLYING, false);
                            mainGirl->Lose();
                            man[level - 1][1][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }

                    if (mainGirl->IsClicked())
                    {
                        mainGirl->Attack(man[level - 1][1][i], &map);

                        for (size_t j = 0; j < girlsOnScreen.size(); j++)
                        {
                            if (girlsOnScreen[j]->IsSpecialGirl())
                                ui.AddScore(15);
                            else
                                ui.AddScore(3);
                        }

                        if (!mainGirl->IsReinforced())
                            ui.AddHeartPoints(-18 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                    {
                        girlsOnScreen[j]->Attack(man[level - 1][1][i], &map);
                    }
                }
                else
                {
                    if (man[level - 1][1][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[level - 1][1][i]))
                    {
                        if (HP <= 0)
                        {
                            man[level - 1][1][i]->SetIsAlive(false);
                            man[level - 1][1][i]->SetIsKilledBy(Man::mainGirl);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            hearts.push_back(new Heart(1, 0, man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2, man[level - 1][1][i]->GetY() - 55, 0));
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }

                        if (man[level - 1][1][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[level - 1][1][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[level - 1][1][i], &map);
                                ui.AddScore(1);
                                ui.AddHeartPoints(-8);

                                if (girlsOnScreen.size() != 0)
                                {
                                    int ready = 0;

                                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                    {
                                        if (girlsOnScreen[j] == NULL)
                                            break;

                                        if (girlsOnScreen[j]->IsLocked())
                                            ready++;

                                        if (girlsOnScreen[j]->GetX() + girlsOnScreen[j]->GetWidth() / 2 >= man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2)
                                            girlsOnScreen[j]->SetDirection(false);
                                        else
                                            girlsOnScreen[j]->SetDirection(true);

                                        girlsOnScreen[j]->LockPerson(man[level - 1][1][i], &map);
                                        girlsOnScreen[j]->SetIsShocking(true);
                                    }

                                    if (ready == girlsOnScreen.size() && mainGirl->IsAttacking())
                                    {
                                        man[level - 1][1][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true);
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false);
                                    }
                                }
                            }
                        }
                        else
                        {
                            man[level - 1][1][i]->SetIsFocused(false);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }
                    }
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < girl[level - 1][0].size(); i++)
        {
            girl[level - 1][0][i]->OnMove(&map, rand());
        }

        for (size_t i = 0; i < girl[level - 1][1].size(); i++)
        {
            girl[level - 1][1][i]->OnMove(&map, rand());
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // special mode
        if (!ui.IsGameOver() && !mainGirl->IsReinforced() && ui.GetHeartPoints() == 4500)
        {
            ui.Pause();
            ui.GotoHRState(CHeartPoint::reinforcing);
            mainGirl->SetIsReinforced(true);
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Pause();
            CAudio::Instance()->Play(AUDIO_REINFORCING, false);
        }

        for (size_t i = 0; i < hearts.size(); i++)
        {
            if (hearts[i]->HitMainGirl(mainGirl))
            {
                //do something like increasing score
                ui.AddScore(hearts[i]->GetHP());

                if (!mainGirl->IsReinforced())
                    ui.AddHeartPoints(hearts[i]->GetHP() / 2 + 300);

                CAudio::Instance()->Play(AUDIO_EAT_HEART, false);
                delete hearts[i];
                hearts.erase(hearts.begin() + i);
                break;
            }
            else
                hearts[i]->OnMove();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (mainGirl->IsReinforced() && ui.GetHeartPoints() == 0)
        {
            ui.GotoHRState(CHeartPoint::normal);
            ui.SetHeartPoints(2000);
            mainGirl->SetIsReinforced(false);
        }

        if (ui.IsGameOver() && mainGirl->IsReinforced())
        {
            ui.GotoHRState(CHeartPoint::normal);
            mainGirl->SetIsReinforced(false);
        }

        if (ui.IsGameOver() && !ui.IsWin() && mainGirl->IsInAnimation())
        {
            static int counter = 280;

            if (counter <= 0)
            {
                *score = ui.GetScore();
                counter = 280;
                ChangeGameState(GAME_STATE_OVER);
            }

            counter--;
        }
        else if (ui.IsGameOver() && ui.IsWin() && mainGirl->IsInAnimation() && !mainGirl->IsReporting())
        {
            static bool isPlayed = false;
            static int counter = 20;

            if (ui.GetHeartPoints() > 0)
            {
                if (!isPlayed)
                {
                    CAudio::Instance()->Play(AUDIO_SUMMARIZE, true);
                    isPlayed = true;
                }
                else
                    counter--;

                if (counter <= 0)
                {
                    ui.AddHeartPoints(-5);
                    ui.AddScore(5);
                }
            }
            else
            {
                if (isPlayed)
                {
                    CAudio::Instance()->Stop(AUDIO_SUMMARIZE);
                    isPlayed = false;
                    counter = 40;
                }
                else
                    counter--;

                if (counter <= 0)
                {
                    counter = 20;
                    *score = ui.GetScore();
                    ChangeGameState(GAME_STATE_OVER);
                }
            }
        }

        mainGirl->OnMove(&map, &ui);

        if (!ui.IsGameOver())
        {
            if (map.IsEmpty(mainGirl->GetPositionX(), mainGirl->GetPositionY()) && map.IsEmpty(mainGirl->GetPositionX() + mainGirl->Width(), mainGirl->GetPositionY()))
            {
                ui.SetIsButtonVisible(false, false);
                ui.SetIsButtonVisible(false, true);
            }
            else
            {
                if (mainGirl->GetPositionX() <= map.Width() / 2)
                    ui.SetIsButtonVisible(true, false);
                else
                    ui.SetIsButtonVisible(true, true);
            }
        }
    }

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭

        if (nChar == KEY_UP)
        {
            ui.SetHeartPoints(4500);
        }
        else if (nChar == KEY_DOWN)
        {
            if (mainGirl->IsReinforced() && !mainGirl->IsInAnimation())
                ui.SetHeartPoints(0);
        }
        else if (nChar == KEY_RIGHT)
        {
            ui.AddScore(1000);
        }
        else if (nChar == KEY_LEFT)
        {
            ui.SetIsGameOver(true);
        }
    }
    void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭
    }
    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
        if (mainGirl->IsFocusing())
        {
            mainGirl->SetIsAttacking(true);

            if (mainGirl->IsLocked() && !mainGirl->IsClicked())
                mainGirl->Click();
            else
                CAudio::Instance()->Play(AUDIO_LASER, true);
        }
    }
    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        if (!mainGirl->IsLocked())
        {
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Stop(AUDIO_LASER);
        }

        if (ui.IsAudioButtonHoverd())
            ui.Toggle();

        if (map.GetLevel() != 4 && ui.IsUpButtonHoverd())
            map.SetLevel(map.GetLevel() + 1);
        else if (map.GetLevel() != 1 && ui.IsDownButtonHoverd())
            map.SetLevel(map.GetLevel() - 1);
    }
    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
        mainGirl->OnMouseMove(point);
        ui.OnMouseMove(point);
    }
    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
        //waveOutSetVolume(0, 0x0000);
        //CAudio::Instance()->Pause();
    }

    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        //waveOutSetVolume(0, 0x0FFF);
        //CAudio::Instance()->Resume();
    }

    void CGameStateRun::OnShow()
    {
        //
        //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
        //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
        //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
        //
        //
        //  貼上背景圖、撞擊數、球、擦子、彈跳的球
        //
        int level = map.GetLevel();
        map.OnShow();
        ui.OnShow(&map);

        for (size_t i = 0; i < man[level - 1][0].size(); i++)
        {
            if (!man[level - 1][0][i]->IsFollowing(Man::mainGirl))
                man[level - 1][0][i]->OnShow(&map);
        }

        for (size_t i = 0; i < girl[level - 1][0].size(); i++)
        {
            girl[level - 1][0][i]->OnShow(&map);
        }

        teacher->OnShow(&map);
        mainGirl->OnShow(&map, &ui);

        for (size_t i = 0; i < hearts.size(); i++)
            hearts[i]->OnShow(&map);

        for (size_t i = 0; i < girl[level - 1][1].size(); i++)
        {
            girl[level - 1][1][i]->OnShow(&map);
        }

        for (size_t i = 0; i < man[level - 1][1].size(); i++)
        {
            if (!man[level - 1][1][i]->IsFollowing(Man::mainGirl))
                man[level - 1][1][i]->OnShow(&map);
        }

        mainGirl->ShowFocus();

        if (ui.IsGameOver())
        {
            static int counter = 120;

            if (!ui.IsWin())
            {
                CRect rect;
                CDDraw::GetClientRect(rect);
                HBITMAP bitmap;
                CDC bkDC;
                CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
                bkDC.CreateCompatibleDC(pDC);
                bitmap = CreateCompatibleBitmap(bkDC.m_hDC, rect.Width(), rect.Height());
                HBITMAP* pOldBitmap = (HBITMAP*)bkDC.SelectObject(bitmap);
                bkDC.SetBkColor(RGB(0, 0, 0));
                BLENDFUNCTION bf;
                bf.AlphaFormat = 0;
                bf.BlendFlags = 0;
                bf.BlendOp = 0;
                bf.SourceConstantAlpha = 200;
                pDC->AlphaBlend(0, 0, rect.Width(), rect.Height(), &bkDC, 0, 0, rect.Width(), rect.Height(), bf);
                bkDC.SelectObject(pOldBitmap);
                bkDC.DeleteDC();

                if (counter <= 20)
                {
                    CFont f, *fp;
                    CSize size;
                    f.CreatePointFont(6000 * counter, "Times New Roman");	// 產生 font f; 160表示16 point的字
                    LOGFONT logFont;
                    f.GetLogFont(&logFont);
                    logFont.lfWeight = FW_BOLD;
                    f.DeleteObject();
                    f.CreatePointFontIndirect(&logFont);
                    fp = pDC->SelectObject(&f);					// 選用 font f
                    size = pDC->GetTextExtent("死");
                    pDC->SetBkMode(TRANSPARENT);
                    char str[80];								// Demo 數字對字串的轉換
                    sprintf(str, "死");
                    pDC->SetTextColor(RGB(187, 13, 13));
                    pDC->TextOut(rect.CenterPoint().x - size.cx / 2 - 3, rect.CenterPoint().y - size.cy / 2 - 3, str);
                    pDC->SetTextColor(RGB(255, 0, 0));
                    pDC->TextOut(rect.CenterPoint().x - size.cx / 2, rect.CenterPoint().y - size.cy / 2, str);
                    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
                }

                if (counter > 1)
                    counter--;

                CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
            }
            else
                counter = 120;
        }

        if (map.IsMapChanging())
            CDDraw::BltBackColor(RGB(0, 0, 0));
    }

    void CGameStateRun::GenerateGoldBoy(int level, bool direction, bool top)
    {
        int mx, my;

        if (top)
        {
            my = 100;
        }
        else
        {
            my = 400;
        }

        mx = rand() % 2030 + 450;
        int distance = 100;

        if (direction)
            man[level][!top].push_back(new SpecialMan(mx, my, mx, mx + distance, direction, 3));
        else
            man[level][!top].push_back(new SpecialMan(mx, my, mx - distance, mx, direction, 3));

        (*(man[level][!top].crbegin()))->LoadBitMap();
        (*(man[level][!top].crbegin()))->OnBeginState();
        srand((unsigned)time(NULL));
        int thresh = rand() % 2;
        girl[level][thresh].push_back(new SpecialGirl(mx + rand() % 50 - rand() % 50, 120 * (1 - thresh) + 380 * (thresh), mx - rand() % 30 - 50, mx + rand() % 30 + 50, rand() % 2, 1));
        (*(girl[level][thresh].crbegin()))->LoadBitMap();
        (*(girl[level][thresh].crbegin()))->OnBeginState();
    }

    void CGameStateRun::LoadData()
    {
        CStdioFile myFile;
        myFile.Open("RES\\data.txt", CFile::modeRead);
        int level = 0;
        CString tmp;
        const std::regex check("^[0-9]?,[0-9]?,[0-9]*,[0-9]*,[0-9]*,[0-9]?,[0-9]?$");
        const std::regex pattern("[^,]+");
        myFile.ReadString(tmp);
        myFile.ReadString(tmp);

        while (level != 4)
        {
            level = stoi(tmp.GetString());

            while (myFile.ReadString(tmp) && std::regex_match(tmp.GetString(), tmp.GetString() + tmp.GetLength(), check))
            {
                string t(tmp.GetString());
                std::regex_iterator<std::string::iterator> rit(t.begin(), t.end(), pattern);
                std::regex_iterator<std::string::iterator> rend;
                vector<int> data;

                while (rit != rend)
                {
                    data.push_back(stoi(rit->str()));
                    rit++;
                }

                //data[0] => top(0) / btm(1) / mid(2)
                //data[1] => 0(girl) / 1(specialGirl) / 2(normalMan) / 3(specialMan) / 4(teacher)
                //data[2] => x
                //data[3] => left
                //data[4] == right
                //data[5] => 0 / 1(direction)
                //data[6] => type
                if (data[1] == 0)
                    girl[level - 1][data[0]].push_back(new NormalGirl(data[2], 120 * (1 - data[0]) + 380 * (data[0]), data[3], data[4], data[5], data[6]));
                else if (data[1] == 2)
                    man[level - 1][data[0]].push_back(new NormalMan(data[2], 100 * (1 - data[0]) + 400 * data[0], data[3], data[4], data[5], data[6]));
                else if (data[1] == 3)
                    man[level - 1][data[0]].push_back(new SpecialMan(data[2], 100 * (1 - data[0]) + 400 * data[0], data[3], data[4], data[5], data[6]));
            }
        }

        myFile.Close();
    }

    void CGameStateRun::ChangeGameState(int state)
    {
        CAudio::Instance()->Stop(AUDIO_GAME);
        GotoGameState(state);
    }
}