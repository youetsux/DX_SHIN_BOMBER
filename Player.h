#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"

class Bomb;

class Player :
    public GameObject
{
    int playerImage_;
    Pointf pos_;
    int maxBomb_;//�{���̍ő�l
    int usedBomb_;//���ݎg�����{���̐� maxBomb - usedBomb����������{����
    void PutBomb(const Point& pos);//pos�ɔ��e��u�����Ƃ���
public:
	Player();
    ~Player();
    Pointf GetPos() { return pos_; }
    void Update() override;
    void Draw() override;
	bool CheckHit(const Rect& me,const Rect& other);
};

//ToDO
//�{���̐��𐧌䂷�镔���������@��
//�{���̏�ɂ̓{����u���Ȃ��B������ ��
//�{���Ƃ������Ƃ����Q�����z��ŊǗ����������y�H�����؁@�H�H�H
