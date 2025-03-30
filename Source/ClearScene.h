#pragma once
#include "../Library/SceneBase.h"
#include "../globals.h"

class ClearScene :
    public SceneBase
{
    int hImage_;
    Rect ImageRect_;
public:
    ClearScene();
    ~ClearScene();
    void Update() override;
    void Draw() override;
};

