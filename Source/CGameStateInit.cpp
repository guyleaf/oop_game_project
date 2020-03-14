#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStateInit.h"

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲開頭畫面物件
    /////////////////////////////////////////////////////////////////////////////

    CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
    {
    }

    void CGameStateInit::OnInit()
    {
        //
        // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
        //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
        //
        ShowInitProgress(0);	// 一開始的loading進度為0%
        //
        // 開始載入資料
        //
        logo.LoadBitmap(IDB_BACKGROUND);
        Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
        //
        // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
        //
    }

    void CGameStateInit::OnBeginState()
    {
    }

    void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        const char KEY_ESC = 27;
        const char KEY_SPACE = ' ';

        if (nChar == KEY_SPACE)
            GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
        else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
    }

    void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
    {
        GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
    }

    void CGameStateInit::OnShow()
    {
        //
        // 貼上logo
        //
        logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
        logo.ShowBitmap();
        //
        // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
        //
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
        CFont f, *fp;
        f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
        fp = pDC->SelectObject(&f);					// 選用 font f
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetTextColor(RGB(255, 255, 0));
        pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
        pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");

        if (ENABLE_GAME_PAUSE)
            pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

        pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
