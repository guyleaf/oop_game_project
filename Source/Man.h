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
		bool moving; //�O�_���b����
		bool direction; //false => ����, true => ���k
		int velocity;
		int score;
		int range[2];
		CAnimation normalMan1_left, normalMan1_right;
		CMovingBitmap normalMan1_left_stand, normalMan1_right_stand;
		//CMovingBitmap girl;
	};
}
