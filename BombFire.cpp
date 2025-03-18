#include "bomb.h"
#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"
#include "ImGui/Imgui.h"
#include <list>

bool BombFire::CheckHitWall(Rect rec)
{
	Stage* stage = (Stage*)FindGameObject<Stage>();
	//	for (auto& obj : stage->GetStageRects())
	//	{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			StageObj& tmp = stage->GetStageGrid()[y][x];
			if (tmp.type == STAGE_OBJ::EMPTY) continue;
			if (CheckHit(rec, tmp.rect))
			{
				if (tmp.type == STAGE_OBJ::BRICK) {
					tmp.isBreak = true;
				}
				//	DrawFormatString(0, 0, GetColor(255, 0, 0), "HIT");
				//ImGui::Begin("config 1");
				//ImGui::Text("(x, y)=(%3d,%3d) TYPE=%1d", x, y, tmp.type);
				//ImGui::End();
				return true;
			}
		}
	}
	return false;
}

bool BombFire::checkHitBomb(Rect rec)
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			std::list<Bomb*> bomList = FindGameObjects<Bomb>();
			for (auto& tmp : bomList)
			{
				if (tmp == nullptr) continue;
				Rect tmpRec = { tmp->GetPos(), CHA_WIDTH, CHA_HEIGHT };
				if (CheckHit(rec, tmpRec))
				{
					tmp->Fire();
					return true;
				}
			}
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
	:GameObject(), pos_({ 0,0 }), isAlive_(true), timer_(140.0f / 60.0f),
	length_(0), iFrame_{ 0,0,0,0 }, isStop{ false,false,false,false }
{
}

BombFire::BombFire(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(140.0f / 60.0f),
	length_(len), iFrame_{ 0,0,0,0 }, isStop{ false,false,false,false }
{
}

BombFire::~BombFire()
{
}

void BombFire::Update()
{
	float dt = Time::DeltaTime();
	timer_ = timer_ - dt;
	if (timer_ < 0) {
		isAlive_ = false;
	}
}

void BombFire::Draw()
{
	Point dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
	if (isAlive_) {
		DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);

		for (int i = 0; i < 4; i++)
		{
			for(int d = 0; d < iFrame_[i]; d++) {
			//for (int d = 1; d <= length_; d++) {
				Point p = { pos_.x + CHA_WIDTH * dirs[i].x * d, pos_.y + CHA_WIDTH * dirs[i].y * d };
				if (CheckHitWall({ p, CHA_WIDTH, CHA_HEIGHT }) || checkHitBomb({p, CHA_WIDTH, CHA_HEIGHT}) ){
					isStop[i] = true;
					//iFrame_[i]--;
					break;
				}
				DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);
			}
		}
		//1フレームに1ブロックずつ伸ばす
		for (int i = 0; i < 4; i++) {
			if (!isStop[i] && iFrame_[i] < length_) {
				iFrame_[i]++;
			}
		}
	}
	else
	{
		this->DestroyMe();
		//DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(0, 0, 255), TRUE);
	}
}
