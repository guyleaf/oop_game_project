#include <vector>
namespace game_framework
{
    class MainGirl
    {
    public:
        MainGirl();
        void LoadBitMap();
        void OnMouseMove(CGameMap* map, CPoint point);
        int GetCursorX();
        int GetCursorY();
        int GetPositionX();
        int GetPositionY();
        int Height();
        int Width();
        void OnMove(CGameMap* map);
        void OnShow(CGameMap* map);
        void ShowFocus();
        void SetIsFocusing(bool status);
        void SetFocusPerson(CGameMap* map, Man* man);
        void SetIsAttacking(bool status);
        bool IsFocusing();
        bool IsFocusPerson(Man* man);
        bool IsAttacking();
        void AddSlave(Man* man);
    private:
        void SetMoving(CGameMap* map, CPoint point);
        void SetVelocity(CGameMap* map, CPoint point);
        void DrawBeam(CGameMap* map);
        int x, y;
        int cursor_x, cursor_y;
        bool moving; //是否正在移動
        bool direction; //false => 往左, true => 往右
        int velocity;
        bool is_focusing;
        bool is_attacking;
        int focus_id;
        int heartPoints;

        CAnimation girl_walk_left, girl_walk_right;
        CAnimation girl_run_left, girl_run_right;
        CMovingBitmap girl_left_stand, girl_right_stand;
        CMovingBitmap girl_left_focusing_behind, girl_left_focusing_front;
        CMovingBitmap girl_right_focusing_behind, girl_right_focusing_front;
        CAnimation focus_point_on, focus_point_off;
        CPoint beam_pos[4];
        vector<Man*> slaves;
    };
}


