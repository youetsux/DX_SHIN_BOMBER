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
    int numBomb_;//ボムの最大値
    int firePower_;//爆風の長さ
    float speed_;//プレイヤーのスピード
    int usedBomb_;//現在使ったボムの数 maxBomb - usedBombが今おけるボム数
    void PutBomb(const Point& pos);//posに爆弾を置こうとする
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
    Pointf deathDir_;
    float timeToDeath_;
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
//ボムの数を制御する部分を実装　◯
//ボムの上にはボムを置けない。を実装 ◯
//ボムとか爆風とかを２次元配列で管理した方が楽？を検証　？？？
