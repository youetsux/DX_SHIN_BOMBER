#pragma once
#include "../Library/SceneBase.h"
#include "../globals.h"

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class TitleScene : public SceneBase
{
	int hTitleImage_;
	Rect drawRect_;
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;
};
