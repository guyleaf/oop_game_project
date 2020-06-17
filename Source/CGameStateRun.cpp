#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateRun.h"

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
    /////////////////////////////////////////////////////////////////////////////

    CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
    {
        mainGirl = new MainGirl();
        normalGirl[0].push_back(new NormalGirl(800, 120, 800, 1500, true, 1));
        normalGirl[1].push_back(new NormalGirl(1500, 380, 300, 1500, false, 1));
        normalGirl[0].push_back(new NormalGirl(400, 120, 400, 700,  true, 2));
        normalGirl[1].push_back(new NormalGirl(900, 380, 200, 900, false, 2));
        man[0].push_back(new NormalMan(120, 100, 120, 300, true, 1));
        man[0].push_back(new NormalMan(500, 100, 500, 1000, true, 2));
        man[0].push_back(new NormalMan(1700, 100, 1700, 2000, true, 1));
        man[1].push_back(new NormalMan(1800, 400, 1000, 1800, false, 1));
        man[1].push_back(new NormalMan(1100, 400, 500, 1100, false, 2));
        man[1].push_back(new NormalMan(1000, 400, 200, 1000, false, 3));
    }

    CGameStateRun::~CGameStateRun()
    {
        for (size_t i = 0; i < man[0].size(); i++)
        {
            delete man[0][i];
        }

        for (size_t i = 0; i < man[1].size(); i++)
        {
            delete man[1][i];
        }

        delete mainGirl;
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

        for (size_t i = 0; i < man[0].size(); i++)
        {
            man[0][i]->LoadBitMap();
        }

        for (size_t i = 0; i < man[1].size(); i++)
        {
            man[1][i]->LoadBitMap();
        }

        for (size_t i = 0; i < normalGirl[0].size(); i++)
        {
            normalGirl[0][i]->LoadBitMap();
        }

        for (size_t i = 0; i < normalGirl[1].size(); i++)
        {
            normalGirl[1][i]->LoadBitMap();
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
        CAudio::Instance()->Load(AUDIO_WARNING, "sounds\\warning.mp3");
        //
        // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
        //
    }

    void CGameStateRun::OnBeginState()
    {
        const int HITS_LEFT = 10;
        const int HITS_LEFT_X = 590;
        const int HITS_LEFT_Y = 0;
        /*hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標*/
        ui.LoadVolume();
        //CAudio::Instance()->Play(AUDIO_GAME, true);			// 撥放 GAME
    }

    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        //
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        //
        //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        //mainGirl->SetIsFocusing(false);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ui.OnMove();

        if (!mainGirl->IsInAnimation() && mainGirl->IsReinforced() && !ui.IsGameOver())
        {
            ui.GotoHRState(CHeartPoint::reinforced);
            ui.Resume();
        }

        if (!mainGirl->IsInAnimation())
        {
            for (size_t i = 0; i < man[0].size(); i++)
            {
                if (man[0][i]->IsAlive() && !mainGirl->IsFocusing() && man[0][i]->HitMainGirl(&map, mainGirl))
                {
                    man[0][i]->SetIsFocused(true);
                    man[0][i]->SetMoving(false);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[0][i]);
                }
                else if (!man[0][i]->IsFocused())
                    man[0][i]->SetMoving(true);
            }

            for (size_t i = 0; i < man[1].size(); i++)
            {
                if (man[1][i]->IsAlive() && !mainGirl->IsFocusing() && man[1][i]->HitMainGirl(&map, mainGirl))
                {
                    man[1][i]->SetIsFocused(true);
                    man[1][i]->SetMoving(false);
                    mainGirl->SetIsFocusing(true);
                    mainGirl->SetFocusPerson(&map, man[1][i]);
                }
                else if (!man[1][i]->IsFocused())
                    man[1][i]->SetMoving(true);
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (vector<Man*>::iterator person = man[0].begin(); person != man[0].end(); person++)
        {
            if ((*person)->IsOver())
            {
                delete (*person);
                person = man[0].erase(person);

                if (person != man[0].end())
                    continue;
                else
                    break;
            }

            (*person)->OnMove();
        }

        for (vector<Man*>::iterator person = man[1].begin(); person != man[1].end(); person++)
        {
            if ((*person)->IsOver())
            {
                delete (*person);
                person = man[1].erase(person);

                if (person != man[1].end())
                    continue;
                else
                    break;
            }

            (*person)->OnMove();
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (mainGirl->IsAttacking() && girlsOnScreen.size() == 0)
        {
            for (size_t i = 0; i < normalGirl[0].size(); i++)
            {
                if (map.IsInScreen(normalGirl[0][i]->GetX(), normalGirl[0][i]->GetX() + normalGirl[0][i]->GetWidth()) && normalGirl[0][i]->IsAlive())
                {
                    girlsOnScreen.push_back(normalGirl[0][i]);
                }
            }

            for (size_t i = 0; i < normalGirl[1].size(); i++)
            {
                if (map.IsInScreen(normalGirl[1][i]->GetX(), normalGirl[1][i]->GetX() + normalGirl[1][i]->GetWidth()) && normalGirl[1][i]->IsAlive())
                {
                    girlsOnScreen.push_back(normalGirl[1][i]);
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
                girlsOnScreen[i]->SetMoving(true);
            }

            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < man[0].size(); i++)
        {
            int HP = int(man[0][i]->GetHP());

            if (man[0][i]->IsAlreadyDead())
            {
                if (HP == 0)
                {
                    mainGirl->AddSlave(man[0][i]);
                    man[0].erase(man[0].begin() + i);
                    break;
                }
            }

            if (!mainGirl->IsInAnimation())
            {
                if (man[0][i]->IsAttackedBy(Man::all) && man[0][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (HP == 800 || HP == 0)
                    {
                        man[0][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP == 0)
                        {
                            hearts.push_back(new Heart(0, 1, man[0][i]->GetX() + man[0][i]->GetWidth() / 2, man[0][i]->GetY(), girlsOnScreen.size()));

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Lose();

                            man[0][i]->SetIsKilledBy(Man::mainGirl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }
                        else if (HP == 800)
                        {
                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Win();

                            ui.AddHeartPoints(-750);
                            mainGirl->Lose();
                            man[0][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }

                    if (mainGirl->IsClicked())
                    {
                        mainGirl->Attack(man[0][i], &map);
                        ui.AddScore(3 * girlsOnScreen.size());
                        ui.AddHeartPoints(-18 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                    {
                        girlsOnScreen[j]->Attack(man[0][i], &map);
                    }
                }
                else
                {
                    if (man[0][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[0][i]))
                    {
                        if (HP == 0)
                        {
                            man[0][i]->SetIsAlive(false);
                            man[0][i]->SetIsKilledBy(Man::mainGirl);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            hearts.push_back(new Heart(0, 0, man[0][i]->GetX() + man[0][i]->GetWidth() / 2, man[0][i]->GetY(), 0));
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }

                        if (man[0][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[0][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[0][i], &map);
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

                                        if (girlsOnScreen[j]->GetX() >= man[0][i]->GetX() + man[0][i]->GetWidth() / 2)
                                            girlsOnScreen[j]->SetDirection(false);
                                        else
                                            girlsOnScreen[j]->SetDirection(true);

                                        girlsOnScreen[j]->LockPerson(man[0][i], &map);
                                        girlsOnScreen[j]->SetIsShocking(true);
                                        girlsOnScreen[j]->SetMoving(false);
                                    }

                                    if (ready == girlsOnScreen.size() && mainGirl->IsAttacking())
                                    {
                                        man[0][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true);
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false);
                                    }
                                }
                            }
                        }
                        else
                        {
                            man[0][i]->SetIsFocused(false);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < man[1].size(); i++)
        {
            int HP = int(man[1][i]->GetHP());

            if (man[1][i]->IsAlreadyDead())
            {
                if (HP == 0)
                {
                    mainGirl->AddSlave(man[1][i]);
                    man[1].erase(man[1].begin() + i);
                    break;
                }
            }

            if (!mainGirl->IsInAnimation())
            {
                if (man[1][i]->IsAttackedBy(Man::all) && man[1][i]->IsAlive() && mainGirl->IsAttacking())
                {
                    if (HP == 800 || HP == 0)
                    {
                        man[1][i]->SetIsAlive(false);
                        mainGirl->SetIsFocusing(false);
                        mainGirl->SetIsAttacking(false);
                        mainGirl->SetIsLocked(false);

                        if (HP == 0)
                        {
                            hearts.push_back(new Heart(1, 1, man[1][i]->GetX() + man[1][i]->GetWidth() / 2, man[1][i]->GetY() - 55, girlsOnScreen.size()));

                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Lose();

                            man[1][i]->SetIsKilledBy(Man::mainGirl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }
                        else if (HP == 800)
                        {
                            for (size_t j = 0; j < girlsOnScreen.size(); j++)
                                girlsOnScreen[j]->Win();

                            ui.AddHeartPoints(-750);
                            mainGirl->Lose();
                            man[1][i]->SetIsKilledBy(Man::girl);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                            girlsOnScreen.erase(girlsOnScreen.begin(), girlsOnScreen.end());
                        }

                        break;
                    }

                    if (mainGirl->IsClicked())
                    {
                        mainGirl->Attack(man[1][i], &map);
                        ui.AddScore(3 * girlsOnScreen.size());
                        ui.AddHeartPoints(-19 * girlsOnScreen.size());
                    }

                    for (size_t j = 0; j < girlsOnScreen.size(); j++)
                    {
                        girlsOnScreen[j]->Attack(man[1][i], &map);
                    }
                }
                else
                {
                    if (man[1][i]->IsAlive() && mainGirl->IsFocusing() && mainGirl->IsFocusPerson(man[1][i]))
                    {
                        if (HP == 0)
                        {
                            man[1][i]->SetIsAlive(false);
                            man[1][i]->SetIsKilledBy(Man::mainGirl);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            hearts.push_back(new Heart(1, 0, man[1][i]->GetX() + man[1][i]->GetWidth() / 2, man[1][i]->GetY() - 55, 0));
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }

                        if (man[1][i]->HitMainGirl(&map, mainGirl))
                        {
                            if (mainGirl->IsAttacking())
                            {
                                man[1][i]->SetIsAttackedBy(Man::mainGirl);
                                mainGirl->Attack(man[1][i], &map);
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

                                        if (girlsOnScreen[j]->GetX() + girlsOnScreen[j]->GetWidth() / 2 >= man[1][i]->GetX() + man[1][i]->GetWidth() / 2)
                                            girlsOnScreen[j]->SetDirection(false);
                                        else
                                            girlsOnScreen[j]->SetDirection(true);

                                        girlsOnScreen[j]->LockPerson(man[1][i], &map);
                                        girlsOnScreen[j]->SetIsShocking(true);
                                        girlsOnScreen[j]->SetMoving(false);
                                    }

                                    if (ready == girlsOnScreen.size() && mainGirl->IsAttacking())
                                    {
                                        man[1][i]->SetIsAttackedBy(Man::all);
                                        mainGirl->SetIsLocked(true);
                                        CAudio::Instance()->Play(AUDIO_SNATCH, false);
                                    }
                                }
                            }
                        }
                        else
                        {
                            man[1][i]->SetIsFocused(false);
                            mainGirl->SetIsFocusing(false);
                            mainGirl->SetIsAttacking(false);
                            CAudio::Instance()->Stop(AUDIO_LASER);
                        }
                    }
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t i = 0; i < normalGirl[0].size(); i++)
        {
            if (normalGirl[0][i]->IsAlreadyDead())
            {
                delete normalGirl[0][i];
                normalGirl[0].erase(normalGirl[0].begin() + i);
                break;
            }

            normalGirl[0][i]->OnMove(&map);
        }

        for (size_t i = 0; i < normalGirl[1].size(); i++)
        {
            if (normalGirl[1][i]->IsAlreadyDead())
            {
                delete normalGirl[1][i];
                normalGirl[1].erase(normalGirl[1].begin() + i);
                break;
            }

            normalGirl[1][i]->OnMove(&map);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // special mode
        if (!mainGirl->IsReinforced() && ui.GetHeartPoints() == 4500)
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

        mainGirl->OnMove(&map);
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

        if (ui.IsAudioButtonHoverd())
            ui.Toggle();
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        if (!mainGirl->IsLocked())
        {
            mainGirl->SetIsAttacking(false);
            CAudio::Instance()->Stop(AUDIO_LASER);
        }
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
        map.OnShow();
        ui.OnShow();

        for (size_t i = 0; i < man[0].size(); i++)
        {
            man[0][i]->OnShow(&map);
        }

        for (size_t i = 0; i < normalGirl[0].size(); i++)
        {
            normalGirl[0][i]->OnShow(&map);
        }

        mainGirl->OnShow(&map);

        for (size_t i = 0; i < hearts.size(); i++)
            hearts[i]->OnShow(&map);

        for (size_t i = 0; i < normalGirl[1].size(); i++)
        {
            normalGirl[1][i]->OnShow(&map);
        }

        for (size_t i = 0; i < man[1].size(); i++)
        {
            man[1][i]->OnShow(&map);
        }

        mainGirl->ShowFocus();
    }
}