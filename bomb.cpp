#include "bomb.h"
#include "Library/Time.h"

Bomb::Bomb()
	:GameObject(),pos_({0,0}),isAlive_(false), timer_(4.0)
{
	
}

Bomb::Bomb(Point pos)
	:GameObject(), pos_({ 0,0 }), isAlive_(true), timer_(3.0f)
{
	pos_ = pos;
}

Bomb::~Bomb()
{
}

void Bomb::Update()
{
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
	else
	{
		//DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);
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
