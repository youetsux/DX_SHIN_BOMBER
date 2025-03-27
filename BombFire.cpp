#include "bomb.h"
#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"
#include "ImGui/Imgui.h"
#include <list>

namespace {
	const float BOMTIMER = 120.0f / 60.0f;
	const int NEIGHBOURS = 9;
	const Point nineNeibor[NEIGHBOURS] = { {0,0}, {1,0}, {0,1}, {1,1}, {-1,0}, {0,-1}, {-1,-1}, {1,-1}, {-1,1} };
	//const Point dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
	const Point dirs[MAXDIR] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, 0} };
	bool isGraphic = true;
}


bool BombFire::CheckHitWall(Rect rec)
{
	Stage* stage = (Stage*)FindGameObject<Stage>();

	for (int i = 0; i < NEIGHBOURS; i++)
	{
		int y = rec.y / CHA_HEIGHT + nineNeibor[i].y;
		int x = rec.x / CHA_WIDTH + nineNeibor[i].x;
		StageObj& tmp = stage->GetStageGrid()[y][x];
		//BOMBも加えたけどいるかな？
		if (tmp.type == STAGE_OBJ::EMPTY || tmp.type == STAGE_OBJ::BOMB) continue;
		if (CheckHit(rec, tmp.rect))
		{
			if (tmp.type == STAGE_OBJ::BRICK) {
				tmp.isBreak = true;
				if(tmp.item != nullptr)
					tmp.item->Exposure();
			}
			return true;
		}
	}

	return false;
}

bool BombFire::checkHitBomb(Rect rec)
{
	//そのままボムだったら？でよくない？

	Stage* stage = (Stage*)FindGameObject<Stage>();
	int x = rec.x / CHA_WIDTH;
	int y = rec.y / CHA_HEIGHT;

	StageObj obj = stage->GetStageGrid()[y][x];

	std::list<Bomb*> bomList = FindGameObjects<Bomb>();
	for (auto& tmp : bomList)
	{
		if (tmp == nullptr) continue;
		int tx = tmp->GetPos().x / CHA_WIDTH;
		int ty = tmp->GetPos().y / CHA_HEIGHT;
		if (tx == x && ty == y)
		{
			tmp->Fire();
			break;
		}
	}

	return false;
}

void BombFire::SetPos(Point pos)
{
	pos_ = pos;
}

Point BombFire::GetPos()
{
	return pos_;
}

BombFire::BombFire()
	:GameObject(), pos_({ 0,0 }), isAlive_(true), timer_(BOMTIMER),
	length_(0), iFrame_{ 0,0,0,0 }, isStop{ false,false,false,false }
{
}

BombFire::BombFire(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(BOMTIMER),
	length_(len), iFrame_{ 0,0,0,0 }, isStop{ false,false,false,false }
{
	if (isGraphic)
		bomFireImage_ = LoadGraph("Assets/bomfire.png");
}

BombFire::~BombFire()
{
}


//なんか方向指定がおかしい気がする。なおす
void BombFire::Update()
{
	bomRectList.clear();
	if (isAlive_) {
		bomRectList.push_back({ { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT }, NONE, false });

		for (int i = 0; i < 4; i++)
		{
			for (int d = 0; d < iFrame_[i]; d++) {
				Point p = { pos_.x + CHA_WIDTH * dirs[i].x * d, pos_.y + CHA_WIDTH * dirs[i].y * d };
				
				if (CheckHitWall({ p, CHA_WIDTH, CHA_HEIGHT }) || checkHitBomb({ p, CHA_WIDTH, CHA_HEIGHT })) {
					//if(d > 0)
					//	bomRectList.back().isEdge = true;
					isStop[i] = true;
					break;
				}
				else
				{
					if(d == iFrame_[i] - 1)
						bomRectList.push_back({ { p.x, p.y, CHA_WIDTH, CHA_HEIGHT }, i, true });
					else
						bomRectList.push_back({ { p.x, p.y, CHA_WIDTH, CHA_HEIGHT }, i, false });
				}
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
	}


	//タイマーの更新
	float dt = Time::DeltaTime();
	timer_ = timer_ - dt;
	if (timer_ < 0) {
		isAlive_ = false;
	}
}

void BombFire::Draw()
{

	int imageNum[MAXDIR + 4]{ 1, 1, 2, 2, 0, 3, 4, 6, 5 };
	if (isAlive_) {
		if (isGraphic)
		{
			for (auto& itr : bomRectList)
			{
				Point p = { itr.rect.x, itr.rect.y };
				if (itr.isEdge)
					DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(100, 15, 12), TRUE);

/*					DrawRectExtendGraph(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT,
						(imageNum[itr.dir + 4]) * 32, 32 * 3, 32, 32,
						bomFireImage_, TRUE);	*/			
				else
					DrawRectExtendGraph(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, 
						imageNum[itr.dir] * 32, 0, 32, 32,
						bomFireImage_, TRUE);
			}

		}
		else
		{
			for (auto& itr : bomRectList)
			{
				Point p = { itr.rect.x, itr.rect.y };
				if (itr.isEdge)
					DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(100, 15, 12), TRUE);
				else
					DrawBox(p.x, p.y, p.x + CHA_WIDTH, p.y + CHA_HEIGHT, GetColor(240, 15, 12), TRUE);
			}
		}
	}
}
