#pragma once
#include "Library/GameObject.h"
#include "globals.h"

class Bomb :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float timer_;
public:
    Bomb();
    Bomb(Point pos);
    ~Bomb();
    void Update() override;
    void Draw() override;
    void SetPos(Point pos);
    Point GetPos();
};

