#include "GameObject.h"
#include <vector>

namespace game_framework
{
    /////////////////////////////////////////////////////////////////////////////
    // 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
    // 每個Member function的Implementation都要弄懂
    /////////////////////////////////////////////////////////////////////////////

    class CGameStateRun : public CGameState
    {
    public:
        CGameStateRun(CGame* g, int* score, bool* isDead);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        void GenerateGoldBoy(int level, bool direction, bool top);
        void LoadData();
        void ChangeGameState(int state);
        CGameMap map;
        MainGirl* mainGirl;
        vector<Man*> man[4][2];
        vector<Girl*> girl[4][2];
        vector<Heart*> hearts;
        vector<Girl*> girlsOnScreen;
        UI ui;
        Teacher* teacher;
        bool isGoldBoyGenerated;
    };
}

