#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"

class Bomb;

class Player :
    public GameObject
{
    int playerImage_;
    Point pos_;
    int maxBomb_;//�{���̍ő�l
    int usedBomb_;//���ݎg�����{���̐� maxBomb - usedBomb����������{����
public:
	Player();
    ~Player();
    Point GetPos() { return pos_; }
    void Update() override;
    void Draw() override;
	bool CheckHit(const Rect& me,const Rect& other);
};

//ToDO
//�{���̐��𐧌䂷�镔��������
//�{���̏�ɂ̓{����u���Ȃ��B������
//�{���Ƃ������Ƃ����Q�����z��ŊǗ����������y�H������
