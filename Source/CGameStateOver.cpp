#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateOver.h"

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的結束狀態(Game Over)
    /////////////////////////////////////////////////////////////////////////////

    CGameStateOver::CGameStateOver(CGame* g)
        : CGameState(g)
    {
    }

    void CGameStateOver::OnMove()
    {
        counter--;

        if (counter < 0)
            GotoGameState(GAME_STATE_INIT);
    }

    void CGameStateOver::OnBeginState()
    {
        counter = 30 * 5; // 5 seconds
    }

    void CGameStateOver::OnInit()
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
        //
        // 開始載入資料
        //
        //Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //
        // 最終進度為100%
        //
        ShowInitProgress(100);
    }

    void CGameStateOver::OnShow()
    {
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CFont f, *fp;
        f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
        fp = pDC->SelectObject(&f);					// 選用 font f
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetTextColor(RGB(255, 255, 0));
        char str[80];								// Demo 數字對字串的轉換
        sprintf(str, "Game Over ! (%d)", counter / 30);
        pDC->TextOut(240, 210, str);
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}