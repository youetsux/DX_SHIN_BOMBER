#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"

bool BombFire::CheckHitWall(Rect rec)
{
	Stage* stage = (Stage*)FindGameObject<Stage>();
	for (auto& obj : stage->GetStageRects())
	{
		if (CheckHit(rec, obj))
		{
			return true;
		}
	}
	return false;
}

void BombFire::SetPos(Point pos)
{
}

Point BombFire::GetPos()
{
	return Point();
}

BombFire::BombFire()
	:GameObject(), pos_({ 0,0 }), isAlive_(true), timer_(140.0f / 60.0f), length_(0), iFrame_(0)
{

}

BombFire::BombFire(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(140.0f / 60.0f), length_(len), iFrame_(0)
{
}

BombFire::~BombFire()
{
}

void BombFire::Update()
{
	float dt = Time::DeltaTime();
	timer_ = timer_ - dt;
	if (timer_ < 0)
		isAlive_ = false;
}

void BombFire::Draw()
{
	Point dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

	if (isAlive_) {
		DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);

		for (int i = 0; i < 4; i++)
		{
			for (int d = 1; d <= iFrame_; d++) {
				Point p = { pos_.x + CHA_WIDTH * dirs[i].x * d, pos_.y + CHA_WIDTH * dirs[i].y * d };
				if (CheckHitWall({ p, CHA_WIDTH, CHA_HEIGHT }))
					break;
				DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);
			}
		}
		if (iFrame_ < length_) {
			iFrame_++;
		}
	}
	//	else
	//	{
	//		for (int i = 0; i < 4; i++)
	//		{
	//			for (int d = 1; d <= length_; d++) {
	//				Point p = { pos_.x + CHA_WIDTH * dirs[i].x * d, pos_.y + CHA_WIDTH * dirs[i].y * d };
	//				if (CheckHitWall({ p, CHA_WIDTH, CHA_HEIGHT }))
	//					break;
	//				DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);
	//			}
	//		}
	//	}
	//}
	else
	{
		this->DestroyMe();
		//DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(0, 0, 255), TRUE);
	}
}
