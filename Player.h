#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"

class Bomb;

enum PLAYER_STATE
{
	PLAYER_ALIVE,
	PLAYER_DEAD_READY,
    PLAYER_DEAD,
    MAX_PLAYER_STATE
};

class Player :
    public GameObject
{
    int playerImage_;
	float animTimer_;
    int animFrame_;
    Pointf pos_;
    int numBomb_;//�{���̍ő�l
    int firePower_;//�����̒���
    float speed_;//�v���C���[�̃X�s�[�h
    int usedBomb_;//���ݎg�����{���̐� maxBomb - usedBomb����������{����
    void PutBomb(const Point& pos);//pos�ɔ��e��u�����Ƃ���
    void GetInputDir();
	DIR inputDir_;
    void MovePlayer(Point _ox, DIR _inputDir, float dt);
    void UpdatePlayerAlive();
    void UpdatePlayerDeadReady();
    void UpdatePlayerDead();
    void PlayerVSItem();
    void PlayerVSEnemy();
    void PlayerVSBombFire();
    //bool isDeadReady_;
    //bool isDead_;
    PLAYER_STATE playerState_;
public:
	Player();
    ~Player();
    Pointf GetPos() { return pos_; }
    void Update() override;
    void Draw() override;
	bool CheckHit(const Rect& me,const Rect& other);
    void FireUP();
    void SpeedUP();
    void BombUP();
};

//ToDO
//�{���̐��𐧌䂷�镔���������@��
//�{���̏�ɂ̓{����u���Ȃ��B������ ��
//�{���Ƃ������Ƃ����Q�����z��ŊǗ����������y�H�����؁@�H�H�H
