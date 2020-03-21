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
    // Constants
    /////////////////////////////////////////////////////////////////////////////

    enum AUDIO_ID  				// 定義各種音效的編號
    {
        AUDIO_DING,				// 0
        AUDIO_LAKE,				// 1
        AUDIO_NTUT				// 2
    };

    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
    /////////////////////////////////////////////////////////////////////////////

    CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
    {
    }

    CGameStateRun::~CGameStateRun()
    {
    }

    void CGameStateRun::OnBeginState()
    {
        const int HITS_LEFT = 10;
        const int HITS_LEFT_X = 590;
        const int HITS_LEFT_Y = 0;
        /*hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
        hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標*/
        CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
        CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
        CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
    }

    void CGameStateRun::OnMove()							// 移動遊戲元素
    {
        //
        // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
        //
        // SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
        girl.OnMove(&map);
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
        girl.LoadBitMap();
        //
        // 完成部分Loading動作，提高進度
        //
        ShowInitProgress(50);
        Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
        CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
        CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
        //
        // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
        //
    }

    void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_LEFT = 0x25; // keyboard左箭頭
        const char KEY_UP = 0x26; // keyboard上箭頭
        const char KEY_RIGHT = 0x27; // keyboard右箭頭
        const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    }

    void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
    }

    void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
        // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
        girl.OnMouseMove(&map, point);
    }

    void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
    {
    }

    void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
    {
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
        map.OnShow(1);
        girl.OnShow(&map);
    }
}