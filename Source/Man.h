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
	protected:
		int x, y;
		//const int height, width;
		bool moving; //是否正在移動
		bool direction; //false => 往左, true => 往右
		int velocity;
		int score;
		int range[2];
		CAnimation normalMan1_left, normalMan1_right, normalMan2_left, normalMan2_right, normalMan3_left, normalMan3_right;
		CMovingBitmap normalMan1_left_stand, normalMan1_right_stand, normalMan2_left_stand, normalMan2_right_stand, normalMan3_left_stand, normalMan3_right_stand;
		//CMovingBitmap girl;
	};

	class Man2 :public Man
	{
	public:
		Man2(int x1, int y1, int start, int end, bool direction);
	};

	class Man3 :public Man
	{
	public:
		Man3(int x2, int y2, int start, int end, bool direction);
		void LoadBitMap();
		void SetMoving(bool status);
		void SetDirection(bool direction);
		void OnMove(CGameMap* map);
		void OnShow(CGameMap* map);
	};
	
	class Man4 :public Man
	{
	public:
		Man4(int x3, int y3, int start, int end, bool direction);
	};

	class Man5 :public Man 
	{
	public:
		Man5(int x4, int y4, int start, int end, bool direction);
		void LoadBitMap();
		void SetMoving(bool status);
		void SetDirection(bool direction);
		void OnMove(CGameMap* map);
		void OnShow(CGameMap* map);
	};
}
