#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include <vector>

using std::vector;

class Enemy :
    public GameObject
{
    Pointf pos_;
    bool isAlive_;
    float speed_;
    Point nextPos_;
    DIR forward_;
    vector<vector<int>> dist;
    vector<vector<Point>> pre;
    bool CheckHit(const Rect& me, const Rect& other);
    bool isHitWall(const Rect& me);

    int enemyImage_;
    float animTimer_;
    int animFrame_;
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
   
    void YCloserMove();
    void XCloserMove();
    void XYCloserMove();
    void XYCloserMoveRandom();
    void RightHandMove();
    void Dijkstra(Point sp, Point gp);

};

