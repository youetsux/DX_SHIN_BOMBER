#include "GameOverScene.h"
#include <DxLib.h>
#include "Input.h"

GameOverScene::GameOverScene()
{
	//hImage_ = LoadGraph("Assets/BomTitle.png");
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update()
{
	if (Input::IsKeyUP(KEY_INPUT_SPACE)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void GameOverScene::Draw()
{
	DrawString(0, 0, "GAMEOVER SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push Space Key To TITLE", GetColor(255, 255, 255));
	//DrawGraph(0, 0, hImage_, FALSE);
}
