#pragma once
#include "Library/GameObject.h"
#include "globals.h"

class BombFire :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float timer_;
    bool isPut_;
public:
    BombFire();
    ~BombFire();
    void Update() override;
    void Draw() override;
};

