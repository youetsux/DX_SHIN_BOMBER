#include "TitleScene.h"
#include <DxLib.h>
#include "Input.h"

TitleScene::TitleScene()
{
	hTitleImage_ = LoadGraph("Assets/BomTitle.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (Input::IsKeyUP(KEY_INPUT_SPACE)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawString(100, 400, "Push Space Key To Play", GetColor(255, 255, 255));
	DrawGraph(0, 0, hTitleImage_, FALSE);
}
