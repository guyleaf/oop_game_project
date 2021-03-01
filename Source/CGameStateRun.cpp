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
    // �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
    /////////////////////////////////////////////////////////////////////////////

    CGameStateRun::CGameStateRun(CGame* g, int* score, bool* isDead) : CGameState(g, score, isDead) // score, isDead �bGameState�����ǻ�
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

    void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
    {
        ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
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

    void CGameStateRun::OnBeginState() // ��l�ƪ��A
    {
        ui.OnBeginState();
        ui.LoadVolume();
        map.OnBeginState();
        mainGirl->OnBeginState();

        for (int level = 0; level < 4; level++) // ���F��l�ƥ~�A�R�����v�k�ͤk��
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
        isGoldBoyShowUp = false; // ���v�k�͹w�]�����
        *isDead = false;
        *score = 0;
    }

    void CGameStateRun::OnMove()							// ���ʹC������
    {
        int level = map.GetLevel(); // ���o�ثe�Ӽh
        map.OnMove();
        ui.OnMove();
        srand((unsigned)time(NULL));

        if (!mainGirl->IsInAnimation() && mainGirl->IsReinforced()) // ��k�D���i�J�S��ɶ��ɡA���v�k�k�X�{
            isGoldBoyShowUp = true;

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation() && mainGirl->IsReinforced()) // �S��ɶ��L���ʵe����
        {
            ui.GotoHRState(CHeartPoint::reinforced);
            ui.Resume();
        }

        if (ui.IsGameOver()) // ��C�������ɡA�Ѱ��Ҧ����A
        {
            if (mainGirl->IsAttacking())
                CAudio::Instance()->Stop(AUDIO_LASER);

            mainGirl->SetIsLocked(false);
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
        }

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation()) // �C���������P�k�D�����b�ʵe�����
        {
            static int delay_counter = -1;

            if (teacher->IsInLevel(level) && teacher->HitMainGirl(mainGirl) && delay_counter == -1) // �Ѯv�P�k�D���ۼ�
            {
                if (mainGirl->IsAttacking()) // ���_�ثe����
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);

                if (!mainGirl->IsReinforced()) // ���b�S��ɶ�
                    ui.AddHeartPoints(-600);

                CAudio::Instance()->Play(AUDIO_FLYING, false); // �u������
                mainGirl->Lose(); // �k�D������
                delay_counter = 30;
            }
            else if (mainGirl->IsAttacking() && !mainGirl->IsReinforced() && ui.GetHeartPoints() <= 0) // �m�ܤ��A�R�߶q�S�F
            {
                if (mainGirl->IsAttacking()) // ���_�ثe����
                    CAudio::Instance()->Stop(AUDIO_LASER);

                mainGirl->SetIsLocked(false);
                mainGirl->SetIsFocusing(false);
                mainGirl->SetIsAttacking(false);
                CAudio::Instance()->Play(AUDIO_FLYING, false); // �u������
                mainGirl->Lose(); // �k�D������
            }

            if (delay_counter != -1)
                delay_counter--;
        }

        teacher->OnMove(&map);

        if (!ui.IsGameOver() && !mainGirl->IsInAnimation())
        {
            // �Ω�k�D����w�k�ͪ��P�_
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
        else // �Ω��C�������ζi�J�L���ʵe�ɡA�ΨӨ���k�ͳQ�k�D���ꦺ
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

        //////////////////////////////////////////////////////    �k�ͳ���    /////////////////////////////////////////////////////////
        for (int i = 0; i < 4; i++) // �Ӽh
        {
            for (vector<Man*>::iterator person = man[i][0].begin(); person != man[i][0].end(); person++)
            {
                if (!isGoldBoyShowUp && (*person)->GetScore() == 40000) // �٩|��Ĳ�o���v�X�{�ɡA���L���v
                    continue;

                if (!(*person)->IsFollowing(Man::mainGirl)) // ���F�Q�k�D�����A�����~�A��l���`�B�@
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

        ////////////////////////////////////////////////////////    �k�D�������ɡA�d�ݪ��񦳵L�k��    /////////////////////////////////////////////////////////////

        if (mainGirl->IsAttacking() && girlsOnScreen.size() == 0) // �[�J�k�ͦ�girlsOnScreen�}�C�A�P�k�Ͷi��m��
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

            if (girlsOnScreen.size() == 0) // ���S�����ܫh��m�@��NULL
            {
                girlsOnScreen.push_back(NULL);
            }
        }
        else if (!mainGirl->IsAttacking()) // �S�������k�ͮɡA�M���}�C
        {
            for (size_t i = 0; i < girlsOnScreen.size(); i++)
            {
                if (girlsOnScreen[i] == NULL)
                    break;

                girlsOnScreen[i]->SetIsShocking(false);
            }

            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
        }

        /////////////////////////////////////////////////////   �B�z�����k�ͻP�k�ͷm�ܪ��޿賡��   ////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < man[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && man[level - 1][0][i]->GetScore() == 40000) // �@�ˡA���v�k���٨SĲ�o�X�{�ɡA�@�߸��L
                continue;

            // �k�ͦ��`�ᤧ�B�z
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
                    // ��Ĺ�M�w������
                    if (HP >= 800 || HP <= 0)
                    {
                        man[level - 1][0][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP <= 0) // �k�D��Ĺ
                        {
                            int num = 0;

                            for (size_t j = 0; j < girlsOnScreen.size(); j++) // �p��k�ͤH��
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
                        else if (HP >= 800) // �k��Ĺ
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

                // �k�D���P�k�ͷm�ܨk�ͪ�����
                if (mainGirl->IsLocked() && man[level - 1][0][i]->IsAttackedBy(Man::all) && man[level - 1][0][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (mainGirl->IsClicked()) // �k�D������ clickĲ�o�O��MouseUp����Ĳ�o
                    {
                        mainGirl->Attack(man[level - 1][0][i], &map);

                        for (size_t j = 0; j < girlsOnScreen.size(); j++)
                        {
                            if (girlsOnScreen[j]->IsSpecialGirl())
                                ui.AddScore(15);
                            else
                                ui.AddScore(3);
                        }

                        if (!mainGirl->IsReinforced()) // �D�S��ɶ��A�Cclick�@����18
                            ui.AddHeartPoints(-18 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++) // �k�ͧ���
                    {
                        girlsOnScreen[j]->Attack(man[level - 1][0][i], &map);
                    }
                }
                else // ��H�k�D����������
                {
                    if (man[level - 1][0][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[level - 1][0][i]))
                    {
                        // �O�_���Q�k�D����w
                        if (man[level - 1][0][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[level - 1][0][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[level - 1][0][i], &map);
                                ui.AddScore(1);
                                ui.AddHeartPoints(-8);

                                if (girlsOnScreen.size() != 0) // �p�G����L�k�ͦb�AĲ�o�m�ܨƥ�
                                {
                                    int ready = 0;

                                    for (vector<Girl*>::iterator person = girlsOnScreen.begin(); person != girlsOnScreen.end(); person++) // ��Ҧ��b�����k�ͳ��ǳƦn�ɡA���ܤk�D���P�k�ͷm�ܨk�ͪ�����
                                    {
                                        if ((*person) == NULL)
                                            break;

                                        // ���v�k�ͪ��W�h�A�u�m�ܪ��v�k�͡A��l���m
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

                                        (*person)->LockPerson(man[level - 1][0][i], &map); // �k����w�k��
                                        (*person)->SetIsShocking(true); // �k�͵o�{�k�D�����b�����k��
                                    }

                                    if (ready != 0 && ready == girlsOnScreen.size() && mainGirl->IsAttacking()) // ���ǳƦn�ɡA���ܤk�D���P�k�ͷm�ܨk�ͪ�����
                                    {
                                        man[level - 1][0][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true); // �ꦺ�k�D�� (�ꦺ��ƹ��i���N����)
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false); // ����m�ܭ���
                                    }
                                }
                            }
                        }
                        else // �Ѱ�����
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

        // �P�W���ۦP
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
                    // ��Ĺ�M�w������
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

                                    for (vector<Girl*>::iterator person = girlsOnScreen.begin(); person != girlsOnScreen.end(); person++) // ��Ҧ��b�����k�ͳ��ǳƦn�ɡA���ܤk�D���P�k�ͷm�ܨk�ͪ�����
                                    {
                                        if ((*person) == NULL)
                                            break;

                                        // ���v�k�ͪ��W�h�A�u�m�ܪ��v�k�͡A��l���m
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

                                        (*person)->LockPerson(man[level - 1][1][i], &map); // �k����w�k��
                                        (*person)->SetIsShocking(true); // �k�͵o�{�k�D�����b�����k��
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

        ///////////////////////////////////////////////////////////    �k�ͳ���    //////////////////////////////////////////////////////////
        for (size_t i = 0; i < girl[level - 1][0].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][0][i]->IsSpecialGirl()) // �٩|��Ĳ�o���v�X�{�ɡA���L���v
                continue;

            if (!girl[level - 1][0][i]->IsAlreadyDead()) // �k�ͨS���h���`�ʧ@
                girl[level - 1][0][i]->OnMove(&map, rand());
        }

        for (size_t i = 0; i < girl[level - 1][1].size(); i++)
        {
            if (!isGoldBoyShowUp && girl[level - 1][1][i]->IsSpecialGirl()) // �٩|��Ĳ�o���v�X�{�ɡA���L���v
                continue;

            if (!girl[level - 1][1][i]->IsAlreadyDead()) // �k�ͨS���h���`�ʧ@
                girl[level - 1][1][i]->OnMove(&map, rand());
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // �i�J�S��ɶ����J�f�A��R�߶q�����ɡA�i�J�L���ʵe
        if (!ui.IsGameOver() && !mainGirl->IsReinforced() && ui.GetHeartPoints() == 4500)
        {
            ui.Pause(); // �ɶ��Ȱ�
            ui.GotoHRState(CHeartPoint::reinforcing); // ��R�߭p�������ܶq��
            mainGirl->SetIsReinforced(true);
            // �Ѱ��k�D�����@���ʧ@
            mainGirl->SetIsFocusing(false);
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Pause(); // �Ȱ�����
            CAudio::Instance()->Play(AUDIO_REINFORCING, false); // �����ܨ�����
        }

        // �R�߱����B�z
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
        if (mainGirl->IsReinforced() && ui.GetHeartPoints() == 0) // �S��ɶ��ӺɡA�k�D�����^���`�Ҧ�
        {
            ui.GotoHRState(CHeartPoint::normal);
            ui.SetHeartPoints(2000); // �w�]2000�R�߶q
            mainGirl->SetIsReinforced(false); // �Ѱ�
        }

        if (ui.IsGameOver() && mainGirl->IsReinforced()) // ��C�������ɡA�Ѱ��k�D�����S��ɶ�
        {
            ui.GotoHRState(CHeartPoint::normal);

            if (ui.GetHeartPoints() < 2000)
                ui.SetHeartPoints(2000);

            mainGirl->SetIsReinforced(false);
        }

        mainGirl->OnMove(&map, &ui);

        if (ui.IsGameOver() && !ui.IsWin() && mainGirl->IsInAnimation()) // �C�������A�k�D�����F������
        {
            static int counter = 280; // ����

            if (counter <= 0)
            {
                *isDead = true;
                *score = ui.GetScore();
                counter = 280;
                ChangeGameState(GAME_STATE_OVER);
            }

            counter--;
        }
        else if (ui.IsGameOver() && ui.IsWin() && mainGirl->IsInAnimation() && !mainGirl->IsReporting()) // �C�������A�k�D���٬��۪�����
        {
            static bool isPlayed = false; // �O�_����L���⭵��
            static int counter = 20; // �C20 loops ���@��

            if (ui.GetHeartPoints() > 0) // �ഫ�R�ߦܤ���
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
            else // ���⧹�ɩ���X������Over
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

        // �W�U�ӱ���s������
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

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) // ��L���U��Ĳ�o
    {
        const char KEY_LEFT = 0x25; // keyboard���b�Y
        const char KEY_UP = 0x26; // keyboard�W�b�Y
        const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
        const char KEY_DOWN = 0x28; // keyboard�U�b�Y

        if (nChar == KEY_UP) // �S��ɶ��Ҧ�
        {
            ui.SetHeartPoints(4500);
        }
        else if (nChar == KEY_DOWN) // �Ѱ��S��ɶ�
        {
            if (mainGirl->IsReinforced() && !mainGirl->IsInAnimation())
                ui.SetHeartPoints(0);
        }
        else if (nChar == KEY_RIGHT) // �[��
        {
            ui.AddScore(1000);
        }
        else if (nChar == KEY_LEFT) // �ߧY������Ƶ����C��
        {
            ui.SetIsGameOver(true);
        }
    }

    void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
    {
        if (mainGirl->IsFocusing()) // �k�D�����b��w�k�ͮ�
        {
            mainGirl->SetIsAttacking(true); // �]�w�k�D�����b�����k��

            if (mainGirl->IsLocked() && !mainGirl->IsClicked()) // �p�i�J�m�ܼҦ��A�������I�@�U�����@��
                mainGirl->Click();
            else
                CAudio::Instance()->Play(AUDIO_LASER, true);
        }
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        if (!mainGirl->IsLocked()) // �p���O�m�ܼҦ��ɡA�����}��A�Ѱ�����
        {
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Stop(AUDIO_LASER);
        }

        if (ui.IsAudioButtonHoverd()) // �n�����s
            ui.Toggle();

        if (!map.IsMapChanging()) // ����a�ϧ󴫼Ӽh�L�{�ɡA�s��Ĳ�o
        {
            // �W�U�ӫ��s������
            if (map.GetLevel() != 4 && ui.IsUpButtonHoverd())
                map.SetLevel(map.GetLevel() + 1);
            else if (map.GetLevel() != 1 && ui.IsDownButtonHoverd())
                map.SetLevel(map.GetLevel() - 1);
        }
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
    {
        mainGirl->OnMouseMove(point);
        ui.OnMouseMove(point);
    }

    void CGameStateRun::OnShow()
    {
        int level = map.GetLevel(); // ���o�ثe�Ӽh
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

        if (ui.IsGameOver()) // �C�������ɡA�L�X�����ʵe
        {
            if (!ui.IsWin()) // �k�D����(���`)
            {
                CRect rect;
                CDDraw::GetClientRect(rect);
                HBITMAP bitmap;
                CDC bkDC;
                CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
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
                    f.CreatePointFont(8000, "Times New Roman");	// ���� font f; 160���16 point���r
                    LOGFONT logFont;
                    f.GetLogFont(&logFont);
                    logFont.lfWeight = FW_BOLD;
                    f.DeleteObject();
                    f.CreatePointFontIndirect(&logFont);
                    fp = pDC->SelectObject(&f);					// ��� font f
                    size = pDC->GetTextExtent("��");
                    pDC->SetBkMode(TRANSPARENT);
                    char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
                    sprintf(str, "��");
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
                    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
                    f.DeleteObject();
                }

                CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC

                if (counter > 1)
                    counter--;
            }
        }
        else
            counter = 130;

        if (map.IsMapChanging()) // �󴫦a�ϼӼh�����
            CDDraw::BltBackColor(RGB(0, 0, 0));
    }

    // �ͦ����v�k�ͻP�k��
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

    // Ū���~������ɮרø��J
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

    // ����GameState
    void CGameStateRun::ChangeGameState(int state)
    {
        CAudio::Instance()->Stop(AUDIO_GAME);
        GotoGameState(state);
    }
}