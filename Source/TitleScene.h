#pragma once
#include "../Library/SceneBase.h"
#include "../globals.h"

/// <summary>
/// �^�C�g���V�[��
/// 
/// �^�C�g����\�����āA�L�[����������v���C�V�[���Ɉڍs����B
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
