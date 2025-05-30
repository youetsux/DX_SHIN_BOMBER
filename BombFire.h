#pragma once
#include "Library/GameObject.h"
#include "globals.h"
#include <vector>

struct BomRect
{
    Rect rect;
    int dir;
    bool isEdge;
};


class BombFire :
    public GameObject
{
    std::vector<BomRect> bomRectList;
	int bomFireImage_;
    Point pos_;
    bool isAlive_;
    float timer_;
    int length_;
    int iFrame_[4]; 
    bool isStop[4];
    bool CheckHitWall(Rect rec);
    bool checkHitBomb(Rect rec);
    float animTimer_;
    int animFrame_;
    bool isFirst_;
public:
    void SetPos(Point pos);
    Point GetPos();
    BombFire();
    BombFire(Point pos, int len);
    ~BombFire();
    void Update() override;
    void Draw() override;

	std::vector<BomRect>& GetBomRectList() { return bomRectList; }
};

