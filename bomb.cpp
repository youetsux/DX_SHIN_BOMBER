#include "bomb.h"
#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"


namespace
{
	float BOMB_TIMER = 3.0f;
}

Bomb::Bomb()
	:GameObject(),pos_({0,0}),isAlive_(false), timer_(BOMB_TIMER),length_(0)
{
	
}

Bomb::Bomb(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(BOMB_TIMER), length_(len)
{
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
}

void Bomb::Draw()
{
	if (isAlive_) {
		float t = 0.25 * sin(2.0 * DX_PI * (4.0 - timer_) / 2.0f);
		Point bpos = { pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT / 2 };
		DrawCircle(bpos.x, bpos.y, (CHA_WIDTH / 2) + (CHA_WIDTH / 3.5) * t, GetColor(15, 15, 12), TRUE);
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
	