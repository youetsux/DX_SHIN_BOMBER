#pragma once

#include <vector>
#include "./Library/GameObject.h"
#include "./globals.h"


class Item :
    public GameObject
{
    Point pos_;
    ITEMS type_;
    bool isInBrick_;
    bool isUsed_;
    int hImage_;
public:
    Item(Point pos, ITEMS type);
    Item();
    ~Item();
    void LoadImage();
    void Update() override;
    void Draw() override;
    void SetPos(Point pos);
    Point GetPos();
    ITEMS UseItem();
    void Exposure() { isInBrick_ = false; }//ブロックからアイテムが出てくるよ

};

