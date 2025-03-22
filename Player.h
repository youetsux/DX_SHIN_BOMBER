#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"

class Bomb;

class Player :
    public GameObject
{
    int playerImage_;
    Pointf pos_;
    int maxBomb_;//ボムの最大値
    int usedBomb_;//現在使ったボムの数 maxBomb - usedBombが今おけるボム数
    void PutBomb(const Point& pos);//posに爆弾を置こうとする
public:
	Player();
    ~Player();
    Pointf GetPos() { return pos_; }
    void Update() override;
    void Draw() override;
	bool CheckHit(const Rect& me,const Rect& other);
};

//ToDO
//ボムの数を制御する部分を実装　◯
//ボムの上にはボムを置けない。を実装 ◯
//ボムとか爆風とかを２次元配列で管理した方が楽？を検証　？？？
