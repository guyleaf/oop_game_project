#include <vector>
namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        ~MainGirl();

        void OnBeginState();
        void LoadBitMap();
        void OnMouseMove(CPoint point);
        int GetCursorX();
        int GetCursorY();
        int GetPositionX();
        int GetPositionY();
        int Height();
        int Width();
        void OnMove(CGameMap* map, UI* ui);
        void OnShow(CGameMap* map, UI* ui);
        void Attack(Man* man, CGameMap* map);
        void ShowFocus();
        void SetIsFocusing(bool status);
        void SetFocusPerson(CGameMap* map, Man* man);
        void SetIsAttacking(bool status);
        void SetIsLocked(bool status);
        void SetIsReinforced(bool status);
        bool IsInAnimation();
        bool IsReinforced();
        bool IsLocked();
        bool IsFocusing();
        bool IsFocusPerson(Man* man);
        bool IsAttacking();
        bool IsReporting();
        void Lose();
        void Click();
        bool IsClicked();
        void AddSlave(Man* man);
    private:
        void InitializeReinforcing();
        void SetVelocity(CGameMap* map);
        void DrawBeam(CGameMap* map);
        int x, y;
        int cursor_x, cursor_y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        bool is_focusing;
        bool is_attacking;
        bool is_locked;
        bool is_clicked;
        bool is_interrupted;
        int focus_id;
        bool is_bump;
        bool is_reinforced;
        bool is_reporting;
        int state;
        int delay_counter;
        CAnimation girl_walk_left, girl_walk_right;
        CAnimation girl_run_left, girl_run_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
        CMovingBitmap girl_left_focusing_behind, girl_left_focusing_front;
        CMovingBitmap girl_right_focusing_behind, girl_right_focusing_front;
        CAnimation girl_right_reinforcing, girl_left_reinforcing;
        CAnimation focus_point_on, focus_point_off;
        CAnimation bump_left, bump_right;
        CAnimation reinforcing[2];
        CBitmap m_memBitmap;
        CPoint beam_pos[4];
        vector<Man*> slaves;
        CAnimation surprising_left, surprising_right;
        CAnimation fun;
    };
}


