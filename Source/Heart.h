namespace game_framework
{
    class Heart
    {
    public:
        Heart(int pos, int size, int x, int y, int numberOfPeople);
        void OnMove();
        void OnShow(CGameMap* map);
        bool HitMainGirl(MainGirl* girl); // �O�_�P�k�D����Ĳ
        int GetHP(); // ���o�R�߶q
    private:
        void LoadBitmap();
        int size; // 0=>normal 1=>big
        int HP;	// �R�߶q
        int x, y; // �R�ߦ�m
        int pos; // �W/�U
        CAnimation heart;
    };
}