#include <vector>
namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        ~MainGirl();

        void OnBeginState();											// 初始化狀態
        void LoadBitMap();												// 載入圖片動畫
        void OnMouseMove(CPoint point);									// 處理滑鼠移動
        int GetCursorX();												// 取得滑鼠X座標 (視窗座標)
        int GetCursorY();												// 取得滑鼠Y座標 (視窗座標)
        int GetPositionX();												// 取得女主角位置X座標 (地圖座標)
        int GetPositionY();												// 取得女主角位置Y座標 (地圖座標)
        int Height();													// 女主角身高
        int Width();													// 女主角寬度
        void OnMove(CGameMap* map, UI* ui);								// 處理動作更新邏輯
        void OnShow(CGameMap* map, UI* ui);								// 處理顯示邏輯
        void Attack(Man* man, CGameMap* map);							// 攻擊
        void ShowFocus();												// 顯示準星
        void SetIsFocusing(bool status);								// 設定鎖定狀態 (單人)
        void SetFocusPerson(CGameMap* map, Man* man);					// 設定鎖定的男生
        void SetIsAttacking(bool status);								// 設定攻擊狀態
        void SetIsLocked(bool status);									// 設定鎖死狀態 (與其他女生搶)
        void SetIsReinforced(bool status);								// 設定特殊時間模式
        bool IsInAnimation();											// 是否正在過場動畫
        bool IsReinforced();											// 是否是特殊時間模式
        bool IsLocked();												// 是否鎖死
        bool IsFocusing();												// 是否鎖定
        bool IsFocusPerson(Man* man);									// 是否為鎖定對象
        bool IsAttacking();												// 是否正在攻擊
        bool IsReporting();												// 是否正在結算分數
        void Lose();													// 輸掉
        void Click();													// 鎖死狀態時生效，每按一次攻擊一次
        bool IsClicked();												// 鎖定狀態時生效，是否按下滑鼠
        void AddSlave(Man* man);										// 將搶到的男生，加入奴隸行列
    private:
        void InitializeReinforcing();									// 初始化特殊時間參數
        void SetVelocity(CGameMap* map);								// 設定移動速度
        void DrawBeam(CGameMap* map);									// 畫出雷射
        int x, y;														// 女主角位置 (地圖座標)
        int cursor_x, cursor_y;											// 滑鼠游標位置 (視窗座標)
        bool moving;													//是否正在移動
        bool direction;													//false => 往左, true => 往右
        int velocity;													// 速度
        bool is_focusing;												// 鎖定狀態
        bool is_attacking;												// 攻擊狀態
        bool is_locked;													// 鎖死狀態 (與其他女生搶)
        bool is_clicked;												// 滑鼠按下 (與其他女生搶)
        bool is_interrupted;											// 用於準星顯示，是否被中斷
        int focus_id;													//鎖定對象之ID
        bool is_bump;													// 擊飛狀態
        bool is_reinforced;												// 特殊時間狀態
        bool is_reporting;												// 結算狀態
        int state;														// 狀態，正常and過場動畫
        int delay_counter;												// 特殊時間周圍特效延遲
        CAnimation girl_walk_left, girl_walk_right;
        CAnimation girl_run_left, girl_run_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
        CMovingBitmap girl_left_focusing_behind, girl_left_focusing_front;
        CMovingBitmap girl_right_focusing_behind, girl_right_focusing_front;
        CAnimation girl_right_reinforcing, girl_left_reinforcing;		// 特殊時間過場動畫
        CAnimation focus_point_on, focus_point_off;						// 準星動畫
        CAnimation bump_left, bump_right;								// 擊飛動畫
        CAnimation reinforcing[2];										// 特殊時間動畫
        CBitmap m_memBitmap;											// 變身光束圖片
        CPoint beam_pos[4];												// 雷射座標
        vector<Man*> slaves;
        CAnimation surprising_left, surprising_right;
        CAnimation fun;
    };
}


