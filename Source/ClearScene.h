#pragma once
#include "../Library/SceneBase.h"


class ClearScene :
    public SceneBase
{
    int hImage_;
public:
    ClearScene();
    ~ClearScene();
    void Update() override;
    void Draw() override;
};

