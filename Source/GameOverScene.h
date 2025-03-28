#pragma once
#include "../Library/SceneBase.h"

class GameOverScene : public SceneBase
{
	int hImage_;
public:
	GameOverScene();
	~GameOverScene();
	void Update() override;
	void Draw() override;
};
