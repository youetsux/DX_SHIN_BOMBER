#pragma once
#include "Library/GameObject.h"
#include "globals.h"

class Bomb :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float timer_;
    int length_;

    int bombImage_;
    float animTimer_;
    int animFrame_;
public:
    Bomb();
    Bomb(Point pos, int len);
    ~Bomb();
    void Update() override;
    void Draw() override;
    void SetPos(Point pos);
    Point GetPos();
    void Fire() { isAlive_ = false; }
};

