#include "bomb.h"
#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"


namespace
{
	float BOMB_TIMER = 4.0f;

	const float ANIM_INTERVAL = 0.3f;
	const int frameNum[4] = { 0,1,2,1 };
	bool isGraphic = false;
}

Bomb::Bomb()
	:GameObject(),pos_({0,0}),isAlive_(false), timer_(BOMB_TIMER),length_(0)
{
	if (isGraphic)
		bombImage_ = LoadGraph("Assets/bomb.png");
	animFrame_ = 0;
	animTimer_ = 0;
}

Bomb::Bomb(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(BOMB_TIMER), length_(len)
{
	if (isGraphic)
		bombImage_ = LoadGraph("Assets/bomb.png");
	animFrame_ = 0;
	animTimer_ = 0;
	Stage* stage = (Stage*)FindGameObject<Stage>();
	int x = pos.x / CHA_WIDTH;
	int y = pos.y / CHA_HEIGHT;
	StageObj& tmp = stage->GetStageGrid()[y][x];
	tmp.type = STAGE_OBJ::BOMB;

}

Bomb::~Bomb()
{
	Stage* stage = (Stage*)FindGameObject<Stage>();
	int x = pos_.x / CHA_WIDTH;
	int y = pos_.y / CHA_HEIGHT;
	StageObj& tmp = stage->GetStageGrid()[y][x];
	tmp.type = STAGE_OBJ::EMPTY;
}

void Bomb::Update()
{
	if (!isAlive_) {
		new BombFire(pos_, length_);
		this->DestroyMe();
	}
	float dt = Time::DeltaTime();
	timer_ = timer_ - dt;
	if (timer_ < 0)
		isAlive_ = false;

	//アニメーション更新
	animTimer_ += Time::DeltaTime();
	if (animTimer_ > ANIM_INTERVAL)
	{
		animFrame_ = (animFrame_ + 1) % 4;
		animTimer_ = animTimer_ - ANIM_INTERVAL;
	}
}

void Bomb::Draw()
{
	if (isAlive_) {
		if (isGraphic)
		{
			DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT,
				frameNum[animFrame_] * 32, 0, 32, 32, bombImage_, TRUE);
		}
		else {
			float t = 0.25 * sin(2.0 * DX_PI * (4.0 - timer_) / 2.0f);
			Point bpos = { pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT / 2 };
			DrawCircle(bpos.x, bpos.y, (CHA_WIDTH / 2) + (CHA_WIDTH / 3.5) * t, GetColor(15, 15, 12), TRUE);
		}
	}
}

void Bomb::SetPos(Point pos)
{
	pos_ = pos;
}

Point Bomb::GetPos()
{
	return pos_;
}
	