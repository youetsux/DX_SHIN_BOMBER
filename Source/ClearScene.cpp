#include "ClearScene.h"
#include <DxLib.h>
#include "Input.h"

ClearScene::ClearScene()
	:hImage_(-1)
{

}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	if (Input::IsKeyUP(KEY_INPUT_SPACE)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void ClearScene::Draw()
{
	DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push Space Key To TITLE", GetColor(255, 255, 255));
}
