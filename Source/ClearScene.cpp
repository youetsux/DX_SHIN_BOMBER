#include "ClearScene.h"
#include <DxLib.h>
#include "Input.h"
#include "../Source/Screen.h"


namespace
{
	const int MESS_WIDTH{ 500 };
}


ClearScene::ClearScene()
	:hImage_(-1),ImageRect_(0,0,0,0)
{
	hImage_ = LoadGraph("Assets/stageclear.png");
	///1101 x 273  Pixels 
	int w, h;
	GetGraphSize(hImage_, &w, &h);

	Point SSIZE{ Screen::WIDTH, Screen::HEIGHT };
	Rect sRect{ 0, 0, SSIZE.x - 1, SSIZE.y - 1 };
	Point center = sRect.GetCenter();
	int wt, ht;
	wt = (int)(MESS_WIDTH / 2);
	ht = (int)((h * ((float)MESS_WIDTH / w)) / 2);
	Rect drawRect{ center.x - wt, center.y - ht, center.x + wt , center.y + ht };
	ImageRect_ = drawRect;
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
	DrawExtendGraph(ImageRect_.x, ImageRect_.y,ImageRect_.w, ImageRect_.h,  hImage_, TRUE);
	//DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push Space Key To TITLE", GetColor(255, 255, 255));
}
