namespace game_framework
{
	class Man
	{
	public:
		Man(int x, int y, int start, int end, bool direction);
		void LoadBitMap();
		void SetVelocity(CGameMap* map, CPoint point);
		void SetMoving(bool status);
		void SetDirection(bool direction);
		void OnMove(CGameMap* map);
		void OnShow(CGameMap* map);
	private:
		int x, y;
		//const int height, width;
		bool moving; //是否正在移動
		bool direction; //false => 往左, true => 往右
		int velocity;
		int score;
		int range[2];
		CAnimation normalMan1_left, normalMan1_right;
		CMovingBitmap normalMan1_left_stand, normalMan1_right_stand;
		//CMovingBitmap girl;
	};
}
