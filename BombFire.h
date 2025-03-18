#pragma once
#include "Library/GameObject.h"
#include "globals.h"

class BombFire :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float timer_;
    int length_;
    int iFrame_[4];
    bool isStop[4];
    bool CheckHitWall(Rect rec);
    bool checkHitBomb(Rect rec);
public:
    void SetPos(Point pos);
    Point GetPos();
    BombFire();
    BombFire(Point pos, int len);
    ~BombFire();
    void Update() override;
    void Draw() override;
};

