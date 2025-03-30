#include "TitleScene.h"
#include <DxLib.h>
#include "Input.h"
#include "../Source/Screen.h"

namespace
{
	const float TITLE_WIDTH_RATIO{ 0.6 };
}


TitleScene::TitleScene()
	:hTitleImage_(-1),drawRect_(0,0,0,0)
{
	hTitleImage_ = LoadGraph("Assets/BomTitle.png");

	int w, h;
	GetGraphSize(hTitleImage_, &w, &h);


	Point SSIZE{ Screen::WIDTH, Screen::HEIGHT };
	Rect sRect{ 0, 0, SSIZE.x - 1, SSIZE.y - 1 };
	Point center = sRect.GetCenter();
	int wt, ht;
	wt = (int)(Screen::WIDTH*TITLE_WIDTH_RATIO / 2);
	ht = (int)((h * ((float)Screen::WIDTH * TITLE_WIDTH_RATIO / w)) / 2);
	drawRect_ ={ center.x - wt, center.y - ht - 65, center.x + wt , center.y + ht };
	
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
	//DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	//DrawString(100, 400, "Push Space Key To Play", GetColor(255, 255, 255));
	//DrawGraph(0, 0, hTitleImage_, FALSE);
	DrawExtendGraph(drawRect_.x, drawRect_.y, drawRect_.w, drawRect_.h, hTitleImage_, TRUE);
}
