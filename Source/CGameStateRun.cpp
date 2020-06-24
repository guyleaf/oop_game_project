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

    CGameStateRun::CGameStateRun(CGame* g, int* score, bool* isDead) : CGameState(g, score, isDead) // score, isDead 在GameState之間傳遞
    {
        mainGirl = new MainGirl();
        LoadData();
        teacher = NULL;
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

            for (size_t i = 0; i < hearts[j].size(); i++)
            {
                delete hearts[j][i];
            }
        }

        delete mainGirl;
        delete teacher;
    }

    void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
    {
        ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
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
        ShowInitProgress(50);
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
        CAudio::Instance()->Load(AUDIO_WARNING, "sounds\\warning.mp3");
    }

    void CGameStateRun::OnBeginState() // 初始化狀態
    {
        ui.OnBeginState();
        ui.LoadVolume();
        map.OnBeginState();
        mainGirl->OnBeginState();

        for (int level = 0; level < 4; level++) // 除了初始化外，刪除金髮男生女生
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

            for (size_t i = 0; i < hearts[level].size(); i++)
            {
                delete hearts[level][i];
            }

            hearts[level].erase(hearts[level].begin(), hearts[level].end());
        }

        srand((unsigned int)time(NULL));
        delete teacher;
        teacher = new Teacher(rand() % 4 + 1, 1000, MIDDLE);
        teacher->LoadBitmap();
        GenerateGoldBoy(0, false, rand() % 2);
        isGoldBoyShowUp = false; // 金髮男生預設不顯示
        *isDead = false;
        *score = 0;
    }

    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        int level = map.GetLevel(); // 取得目前樓層
        map.OnMove();
        ui.OnMove();
        srand((unsigned)time(NULL));

        if (!mainGirl->IsInAnimation() && mainGirl->IsReinforced()) // 當女主角進入特殊時間時，金髮男女出現
            isGoldBoyShowUp = true;

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation() && mainGirl->IsReinforced()) // 特殊時間過場動畫結束
        {
            ui.GotoHRState(CHeartPoint::reinforced);
            ui.Resume();
        }

        if (ui.IsGameOver()) // 當遊戲結束時，解除所有狀態
        {
            if (mainGirl->IsAttacking())
                CAudio::Instance()->Stop(AUDIO_LASER);

            mainGirl->SetIsLocked(false);
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
        }

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation()) // 遊戲未結束與女主角不在動畫轉場中
        {
            static int delay_counter = -1;

            if (teacher->IsInLevel(level) && teacher->HitMainGirl(mainGirl) && delay_counter == -1) // 老師與女主角相撞
            {
                if (mainGirl->IsAttacking()) // 中斷目前攻擊
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);

                if (!mainGirl->IsReinforced()) // 不在特殊時間
                    ui.AddHeartPoints(-600);

                CAudio::Instance()->Play(AUDIO_FLYING, false); // 彈飛音效
                mainGirl->Lose(); // 女主角撞飛
                delay_counter = 30;
            }
            else if (mainGirl->IsAttacking() && !mainGirl->IsReinforced() && ui.GetHeartPoints() <= 0) // 搶奪中，愛心量沒了
            {
                if (mainGirl->IsAttacking()) // 中斷目前攻擊
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);
                CAudio::Instance()->Play(AUDIO_FLYING, false); // 彈飛音效
                mainGirl->Lose(); // 女主角撞飛
            }

            if (delay_counter != -1)
                delay_counter--;
        }

        teacher->OnMove(&map);

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation())
        {
            // 用於女主角鎖定男生的判斷
            for (size_t i = 0; i < man[level - 1][0].size(); i++)
            {
                if (!isGoldBoyShowUp && man[level - 1][0][i]->GetScore() == 40000)
                    continue;

                if (man[level - 1][0][i]->IsAlive() && !mainGirl->IsFocusing() && man[level - 1][0][i]->HitMainGirl(&map, mainGirl))
                {
                    man[level - 1][0][i]->SetIsFocused(true);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[level - 1][0][i]);
                }
            }

            for (size_t i = 0; i < man[level - 1][1].size(); i++)
            {
                if (!isGoldBoyShowUp && man[level - 1][1][i]->GetScore() == 40000)
                    continue;

                if (man[level - 1][1][i]->IsAlive() && !mainGirl->IsFocusing() && man[level - 1][1][i]->HitMainGirl(&map, mainGirl))
                {
                    man[level - 1][1][i]->SetIsFocused(true);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[level - 1][1][i]);
                }
            }
        }
        else // 用於當遊戲結束或進入過場動畫時，用來防止男生被女主角鎖死
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

        //////////////////////////////////////////////////////    男生部分    /////////////////////////////////////////////////////////
        for (int i = 0; i < 4; i++) // 樓層
        {
            for (vector<Man*>::iterator person = man[i][0].begin(); person != man[i][0].end(); person++)
            {
                if (!isGoldBoyShowUp && (*person)->GetScore() == 40000) // 還尚未觸發金髮出現時，跳過金髮
                    continue;

                if (!(*person)->IsFollowing(Man::mainGirl)) // 除了被女主角收服的之外，其餘正常運作
                    (*person)->OnMove(rand());
            }

            for (vector<Man*>::iterator person = man[i][1].begin(); person != man[i][1].end(); person++)
            {
                if (!isGoldBoyShowUp && (*person)->GetScore() == 40000)
                    continue;

                if (!(*person)->IsFollowing(Man::mainGirl))
                    (*person)->OnMove(rand());
            }
        }

        ////////////////////////////////////////////////////////    女主角攻擊時，查看附近有無女生    /////////////////////////////////////////////////////////////

        if (mainGirl->IsAttacking() && girlsOnScreen.size() == 0) // 加入女生至girlsOnScreen陣列，與女生進行搶奪
        {
            for (size_t i = 0; i < girl[level - 1][0].size(); i++)
            {
                if (!isGoldBoyShowUp && girl[level - 1][0][i]->IsSpecialGirl())
                    continue;

                if (map.IsInScreen(girl[level - 1][0][i]->GetX() + 13, girl[level - 1][0][i]->GetX() + girl[level - 1][0][i]->GetWidth() - 13) && girl[level - 1][0][i]->IsAlive())
                {
                    girlsOnScreen.push_back(girl[level - 1][0][i]);
                }
            }

            for (size_t i = 0; i < girl[level - 1][1].size(); i++)
            {
                if (!isGoldBoyShowUp && girl[level - 1][1][i]->IsSpecialGirl())
                    continue;

                if (map.IsInScreen(girl[level - 1][1][i]->GetX() + 13, girl[level - 1][1][i]->GetX() + girl[level - 1][1][i]->GetWidth() - 12) && girl[level - 1][1][i]->IsAlive())
                {
                    girlsOnScreen.push_back(girl[level - 1][1][i]);
                }
            }

            if (girlsOnScreen.size() == 0) // 都沒有的話則放置一個NULL
            {
                girlsOnScreen.push_back(NULL);
            }
        }
        else if (!mainGirl->IsAttacking()) // 沒有攻擊男生時，清除陣列
        {
            for (size_t i = 0; i < girlsOnScreen.size(); i++)
            {
                if (girlsOnScreen[i] == NULL)
                    break;

                girlsOnScreen[i]->SetIsShocking(false);
            }

            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
        }

        /////////////////////////////////////////////////////   處理攻擊男生與女生搶奪的邏輯部分   ////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < man[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && man[level - 1][0][i]->GetScore() == 40000) // 一樣，金髮男生還沒觸發出現時，一律跳過
                continue;

            // 男生死亡後之處理
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
            else if (man[level - 1][0][i]->IsAlive())
            {
                int HP = int(man[level - 1][0][i]->GetHP());

                if (man[level - 1][0][i]->IsAttackedBy(Man::all))
                {
                    // 輸贏決定的部分
                    if (HP >= 800 || HP <= 0)
                    {
                        man[level - 1][0][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP <= 0) // 女主角贏
                        {
                            int num = 0;

                            for (size_t j = 0; j < girlsOnScreen.size(); j++) // 計算女生人數
                            {
                                if (girlsOnScreen[j]->IsSpecialGirl())
                                    num += 5;
                                else
                                    num += 1;
                            }

                            hearts[level - 1].push_back(new Heart(0, 1, man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2, man[level - 1][0][i]->GetY(), num));

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Lose();

                            man[level - 1][0][i]->SetIsKilledBy(Man::mainGirl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }
                        else if (HP >= 800) // 女生贏
                        {
                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Win();

                            if (!mainGirl->IsReinforced())
                                ui.AddHeartPoints(-750);

                            CAudio::Instance()->Play(AUDIO_FLYING, false);
                            mainGirl->Lose();
                            man[level - 1][0][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }
                }
                else
                {
                    if (HP <= 0)
                    {
                        man[level - 1][0][i]->SetIsAlive(false);
                        man[level - 1][0][i]->SetIsKilledBy(Man::mainGirl);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        hearts[level - 1].push_back(new Heart(0, 0, man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2, man[level - 1][0][i]->GetY(), 0));
                        CAudio::Instance()->Stop(AUDIO_LASER);
                        break;
                    }
                }
            }

            if (!ui.IsGameOver())
            {
                int HP = int(man[level - 1][0][i]->GetHP());

                // 女主角與女生搶奪男生的部分
                if (mainGirl->IsLocked() && man[level - 1][0][i]->IsAttackedBy(Man::all) && man[level - 1][0][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (mainGirl->IsClicked()) // 女主角攻擊 click觸發是由MouseUp那裡觸發
                    {
                        mainGirl->Attack(man[level - 1][0][i], &map);

                        for (size_t j = 0; j < girlsOnScreen.size(); j++)
                        {
                            if (girlsOnScreen[j]->IsSpecialGirl())
                                ui.AddScore(15);
                            else
                                ui.AddScore(3);
                        }

                        if (!mainGirl->IsReinforced()) // 非特殊時間，每click一次扣18
                            ui.AddHeartPoints(-18 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++) // 女生攻擊
                    {
                        girlsOnScreen[j]->Attack(man[level - 1][0][i], &map);
                    }
                }
                else // 單人女主角攻擊部分
                {
                    if (man[level - 1][0][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[level - 1][0][i]))
                    {
                        // 是否有被女主角鎖定
                        if (man[level - 1][0][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[level - 1][0][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[level - 1][0][i], &map);
                                ui.AddScore(1);
                                ui.AddHeartPoints(-8);

                                if (girlsOnScreen.size() != 0) // 如果有其他女生在，觸發搶奪事件
                                {
                                    int ready = 0;

                                    for (vector<Girl*>::iterator person = girlsOnScreen.begin(); person != girlsOnScreen.end(); person++) // 當所有在場的女生都準備好時，跳至女主角與女生搶奪男生的部分
                                    {
                                        if ((*person) == NULL)
                                            break;

                                        // 金髮女生的規則，只搶奪金髮男生，其餘不搶
                                        if ((*person)->IsSpecialGirl() && man[level - 1][0][i]->GetScore() != 40000)
                                        {
                                            person = girlsOnScreen.erase(person);

                                            if (person == girlsOnScreen.end())
                                                break;
                                        }

                                        if ((*person)->IsLocked())
                                            ready++;

                                        if ((*person)->GetX() >= man[level - 1][0][i]->GetX() + man[level - 1][0][i]->GetWidth() / 2)
                                            (*person)->SetDirection(false);
                                        else
                                            (*person)->SetDirection(true);

                                        (*person)->LockPerson(man[level - 1][0][i], &map); // 女生鎖定男生
                                        (*person)->SetIsShocking(true); // 女生發現女主角正在攻擊男生
                                    }

                                    if (ready != 0 && ready == girlsOnScreen.size() && mainGirl->IsAttacking()) // 都準備好時，跳至女主角與女生搶奪男生的部分
                                    {
                                        man[level - 1][0][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true); // 鎖死女主角 (鎖死後滑鼠可任意移動)
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false); // 播放搶奪音效
                                    }
                                }
                            }
                        }
                        else // 解除攻擊
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

        // 與上面相同
        for (size_t i = 0; i < man[level - 1][1].size(); i++)
        {
            if (!isGoldBoyShowUp && man[level - 1][1][i]->GetScore() == 40000)
                continue;

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
            else if (man[level - 1][1][i]->IsAlive())
            {
                int HP = int(man[level - 1][1][i]->GetHP());

                if (man[level - 1][1][i]->IsAttackedBy(Man::all))
                {
                    // 輸贏決定的部分
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

                            hearts[level - 1].push_back(new Heart(1, 1, man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2, man[level - 1][1][i]->GetY() - 55, num));

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

                            if (!mainGirl->IsReinforced())
                                ui.AddHeartPoints(-750);

                            CAudio::Instance()->Play(AUDIO_FLYING, false);
                            mainGirl->Lose();
                            man[level - 1][1][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }
                }
                else
                {
                    if (HP <= 0)
                    {
                        man[level - 1][1][i]->SetIsAlive(false);
                        man[level - 1][1][i]->SetIsKilledBy(Man::mainGirl);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        hearts[level - 1].push_back(new Heart(1, 0, man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2, man[level - 1][1][i]->GetY() - 55, 0));
                        CAudio::Instance()->Stop(AUDIO_LASER);
                    }
                }
            }

            if (!ui.IsGameOver())
            {
                int HP = int(man[level - 1][1][i]->GetHP());

                if (man[level - 1][1][i]->IsAttackedBy(Man::all) && man[level - 1][1][i]->IsAlive() && mainGirl->IsAttacking())
                {
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

                                    for (vector<Girl*>::iterator person = girlsOnScreen.begin(); person != girlsOnScreen.end(); person++) // 當所有在場的女生都準備好時，跳至女主角與女生搶奪男生的部分
                                    {
                                        if ((*person) == NULL)
                                            break;

                                        // 金髮女生的規則，只搶奪金髮男生，其餘不搶
                                        if ((*person)->IsSpecialGirl() && man[level - 1][1][i]->GetScore() != 40000)
                                        {
                                            person = girlsOnScreen.erase(person);

                                            if (person == girlsOnScreen.end())
                                                break;
                                        }

                                        if ((*person)->IsLocked())
                                            ready++;

                                        if ((*person)->GetX() >= man[level - 1][1][i]->GetX() + man[level - 1][1][i]->GetWidth() / 2)
                                            (*person)->SetDirection(false);
                                        else
                                            (*person)->SetDirection(true);

                                        (*person)->LockPerson(man[level - 1][1][i], &map); // 女生鎖定男生
                                        (*person)->SetIsShocking(true); // 女生發現女主角正在攻擊男生
                                    }

                                    if (ready != 0 && ready == girlsOnScreen.size() && mainGirl->IsAttacking())
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

        ///////////////////////////////////////////////////////////    女生部分    //////////////////////////////////////////////////////////
        for (size_t i = 0; i < girl[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][0][i]->IsSpecialGirl()) // 還尚未觸發金髮出現時，跳過金髮
                continue;

            if (!girl[level - 1][0][i]->IsAlreadyDead()) // 女生沒死則正常動作
                girl[level - 1][0][i]->OnMove(&map, rand());
        }

        for (size_t i = 0; i < girl[level - 1][1].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][1][i]->IsSpecialGirl()) // 還尚未觸發金髮出現時，跳過金髮
                continue;

            if (!girl[level - 1][1][i]->IsAlreadyDead()) // 女生沒死則正常動作
                girl[level - 1][1][i]->OnMove(&map, rand());
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 進入特殊時間的入口，當愛心量全滿時，進入過場動畫
        if (!ui.IsGameOver() && !mainGirl->IsReinforced() && ui.GetHeartPoints() == 4500)
        {
            ui.Pause(); // 時間暫停
            ui.GotoHRState(CHeartPoint::reinforcing); // 把愛心計分切換至量條
            mainGirl->SetIsReinforced(true);
            // 解除女主角的一切動作
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Pause(); // 暫停音樂
            CAudio::Instance()->Play(AUDIO_REINFORCING, false); // 播放變身音效
        }

        // 愛心掉落處理
        for (size_t i = 0; i < hearts[level - 1].size(); i++)
        {
            if (hearts[level - 1][i]->HitMainGirl(mainGirl))
            {
                ui.AddScore(hearts[level - 1][i]->GetHP());

                if (!mainGirl->IsReinforced())
                    ui.AddHeartPoints(hearts[level - 1][i]->GetHP() / 2 + 300);

                CAudio::Instance()->Play(AUDIO_EAT_HEART, false);
                delete hearts[level - 1][i];
                hearts[level - 1].erase(hearts[level - 1].begin() + i);
                break;
            }
            else
                hearts[level - 1][i]->OnMove();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (mainGirl->IsReinforced() && ui.GetHeartPoints() == 0) // 特殊時間耗盡，女主角跳回正常模式
        {
            ui.GotoHRState(CHeartPoint::normal);
            ui.SetHeartPoints(2000); // 預設2000愛心量
            mainGirl->SetIsReinforced(false); // 解除
        }

        if (ui.IsGameOver() && mainGirl->IsReinforced()) // 當遊戲結束時，解除女主角的特殊時間
        {
            ui.GotoHRState(CHeartPoint::normal);

            if (ui.GetHeartPoints() < 2000)
                ui.SetHeartPoints(2000);

            mainGirl->SetIsReinforced(false);
        }

        mainGirl->OnMove(&map, &ui);

        if (ui.IsGameOver() && !ui.IsWin() && mainGirl->IsInAnimation()) // 遊戲結束，女主角死了的部分
        {
            static int counter = 280; // 延遲

            if (counter <= 0)
            {
                *isDead = true;
                *score = ui.GetScore();
                counter = 280;
                ChangeGameState(GAME_STATE_OVER);
            }

            counter--;
        }
        else if (ui.IsGameOver() && ui.IsWin() && mainGirl->IsInAnimation() && !mainGirl->IsReporting()) // 遊戲結束，女主角還活著的部分
        {
            static bool isPlayed = false; // 是否播放過結算音效
            static int counter = 20; // 每20 loops 扣一次

            if (ui.GetHeartPoints() > 0) // 轉換愛心至分數
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
            else // 結算完時延遲幾秒後跳至Over
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
                    *isDead = false;
                    *score = ui.GetScore();
                    ChangeGameState(GAME_STATE_OVER);
                }
            }
        }

        // 上下樓梯按鈕的部分
        if (!ui.IsGameOver() && !mainGirl->IsInAnimation())
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
        else
        {
            ui.SetIsButtonVisible(false, false);
            ui.SetIsButtonVisible(false, true);
        }
    }

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) // 鍵盤按下時觸發
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭

        if (nChar == KEY_UP) // 特殊時間模式
        {
            ui.SetHeartPoints(4500);
        }
        else if (nChar == KEY_DOWN) // 解除特殊時間
        {
            if (mainGirl->IsReinforced() && !mainGirl->IsInAnimation())
                ui.SetHeartPoints(0);
        }
        else if (nChar == KEY_RIGHT) // 加分
        {
            ui.AddScore(1000);
        }
        else if (nChar == KEY_LEFT) // 立即結算分數結束遊戲
        {
            ui.SetIsGameOver(true);
        }
    }

    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
        if (mainGirl->IsFocusing()) // 女主角正在鎖定男生時
        {
            mainGirl->SetIsAttacking(true); // 設定女主角正在攻擊男生

            if (mainGirl->IsLocked() && !mainGirl->IsClicked()) // 如進入搶奪模式，切換成點一下攻擊一次
                mainGirl->Click();
            else
                CAudio::Instance()->Play(AUDIO_LASER, true);
        }
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        if (!mainGirl->IsLocked()) // 如不是搶奪模式時，按鍵放開後，解除攻擊
        {
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Stop(AUDIO_LASER);
        }

        if (ui.IsAudioButtonHoverd()) // 聲音按鈕
            ui.Toggle();

        if (!map.IsMapChanging()) // 防止地圖更換樓層過程時，連按觸發
        {
            // 上下樓按鈕的部分
            if (map.GetLevel() != 4 && ui.IsUpButtonHoverd())
                map.SetLevel(map.GetLevel() + 1);
            else if (map.GetLevel() != 1 && ui.IsDownButtonHoverd())
                map.SetLevel(map.GetLevel() - 1);
        }
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        mainGirl->OnMouseMove(point);
        ui.OnMouseMove(point);
    }

    void CGameStateRun::OnShow()
    {
        int level = map.GetLevel(); // 取得目前樓層
        map.OnShow();
        ui.OnShow(&map);

        for (size_t i = 0; i < man[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && man[level - 1][0][i]->GetScore() == 40000)
                continue;

            if (!man[level - 1][0][i]->IsFollowing(Man::mainGirl))
                man[level - 1][0][i]->OnShow(&map);
        }

        for (size_t i = 0; i < girl[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][0][i]->IsSpecialGirl())
                continue;

            if (!girl[level - 1][0][i]->IsAlreadyDead())
                girl[level - 1][0][i]->OnShow(&map);
        }

        teacher->OnShow(&map);
        mainGirl->OnShow(&map, &ui);

        for (size_t i = 0; i < hearts[level - 1].size(); i++)
            hearts[level - 1][i]->OnShow(&map);

        for (size_t i = 0; i < girl[level - 1][1].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][1][i]->IsSpecialGirl())
                continue;

            if (!girl[level - 1][1][i]->IsAlreadyDead())
                girl[level - 1][1][i]->OnShow(&map);
        }

        for (size_t i = 0; i < man[level - 1][1].size(); i++)
        {
            if (!isGoldBoyShowUp && man[level - 1][1][i]->GetScore() == 40000)
                continue;

            if (!man[level - 1][1][i]->IsFollowing(Man::mainGirl))
                man[level - 1][1][i]->OnShow(&map);
        }

        mainGirl->ShowFocus();
        static int counter = 130;

        if (ui.IsGameOver()) // 遊戲結束時，印出對應動畫
        {
            if (!ui.IsWin()) // 女主角輸(死亡)
            {
                CRect rect;
                CDDraw::GetClientRect(rect);
                HBITMAP bitmap;
                CDC bkDC;
                CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
                //CDDraw::BltBackColor(RGB(0, 0, 0));
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
                    f.CreatePointFont(8000, "Times New Roman");	// 產生 font f; 160表示16 point的字
                    LOGFONT logFont;
                    f.GetLogFont(&logFont);
                    logFont.lfWeight = FW_BOLD;
                    f.DeleteObject();
                    f.CreatePointFontIndirect(&logFont);
                    fp = pDC->SelectObject(&f);					// 選用 font f
                    size = pDC->GetTextExtent("菜");
                    pDC->SetBkMode(TRANSPARENT);
                    char str[80];								// Demo 數字對字串的轉換
                    sprintf(str, "菜");
                    pDC->SetTextColor(RGB(235 + counter, counter, counter));
                    pDC->TextOut(rect.CenterPoint().x - size.cx / 2, rect.CenterPoint().y - size.cy / 2, str);
                    pDC->SelectObject(fp);
                    f.DeleteObject();
                    f.CreatePointFont(200, "Times New Roman");
                    fp = pDC->SelectObject(&f);
                    pDC->SetTextCharacterExtra(2);
                    size = pDC->GetTextExtent("NOOB");
                    sprintf(str, "NOOB");
                    pDC->TextOut(rect.CenterPoint().x - size.cx / 2, rect.CenterPoint().y - size.cy / 2 + 110, str);
                    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
                    f.DeleteObject();
                }

                CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

                if (counter > 1)
                    counter--;
            }
        }
        else
            counter = 130;

        if (map.IsMapChanging()) // 更換地圖樓層之轉場
            CDDraw::BltBackColor(RGB(0, 0, 0));
    }

    // 生成金髮男生與女生
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

    // 讀取外部資料檔案並載入
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

    // 切換GameState
    void CGameStateRun::ChangeGameState(int state)
    {
        CAudio::Instance()->Stop(AUDIO_GAME);
        GotoGameState(state);
    }
}