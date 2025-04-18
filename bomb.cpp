#include "bomb.h"
#include "BombFire.h"
#include "Library/Time.h"
#include "Stage.h"
#include "Player.h"


namespace
{
	const float BOMB_TIMER = 3.0f;
	const float BOM_FIRE_TIMER = 2.0f;
	const float ANIM_INTERVAL = 0.3f;
	const int frameNum[4] = { 0,1,2,1 };
	const float COLLISION_DIST{ 0.99f };

	bool isGraphic = true;

}

Bomb::Bomb()
	:GameObject(),pos_({0,0}),isAlive_(false), timer_(BOMB_TIMER),length_(1),bombImage_(-1)
{
	if (isGraphic)
	bombImage_ = LoadGraph("Assets/bomb.png");
	animFrame_ = 0;
	animTimer_ = 0;
	delTimer_ = BOM_FIRE_TIMER;
}

Bomb::Bomb(Point pos, int len)
	:GameObject(), pos_(pos), isAlive_(true), timer_(BOMB_TIMER), length_(len),bombImage_(-1)
{
	if (isGraphic)
		bombImage_ = LoadGraph("Assets/bomb.png");
	animFrame_ = 0;
	animTimer_ = 0;
	Stage* stage = (Stage*)FindGameObject<Stage>();
	int x = pos.x / CHA_WIDTH;
	int y = pos.y / CHA_HEIGHT;
	CheckBoundary(x, y); //�͈͊O�̏ꍇ�͕␳
	StageObj& tmp = stage->GetStageGrid()[y][x];
	//tmp.type = STAGE_OBJ::BOMB;
	delTimer_ = BOM_FIRE_TIMER;
}

Bomb::~Bomb()
{
	Stage* stage = (Stage*)FindGameObject<Stage>();
	int x = pos_.x / CHA_WIDTH;
	int y = pos_.y / CHA_HEIGHT;
	CheckBoundary(x, y); //�͈͊O�̏ꍇ�͕␳
	StageObj& tmp = stage->GetStageGrid()[y][x];
	tmp.type = STAGE_OBJ::EMPTY;
}

void Bomb::Update()
{
	
	Player* p = FindGameObject<Player>();
	int x = pos_.x / CHA_WIDTH;
	int y = pos_.y / CHA_HEIGHT;
	Rect bRec{ x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };
	Rect pRect{ (int)p->GetPos().x, (int)p->GetPos().y, CHA_WIDTH, CHA_HEIGHT};
	Point bc = bRec.GetCenter();

	Point playerCenter = pRect.GetCenter();
	//float dist = (bc.x - playerCenter.x) * (bc.x - playerCenter.x) + (bc.y - playerCenter.y) * (bc.y - playerCenter.y);
	float dist = CalcDistance(bc, playerCenter);
	
	if (dist > COLLISION_DIST * CHA_WIDTH)
	{
		Stage* s = (Stage*)FindGameObject<Stage>();
		CheckBoundary(x, y); //�͈͊O�̏ꍇ�͕␳
		StageObj& tmp = s->GetStageGrid()[y][x];
		if (tmp.type == STAGE_OBJ::EMPTY)
			tmp.type = STAGE_OBJ::BOMB;
		tmp.type = STAGE_OBJ::BOMB;
	}
	
	//���e���A�ΉԂɂȂ�
	if (!isAlive_) {
		new BombFire(pos_, length_);
		this->DestroyMe();
	}
	float dt = Time::DeltaTime();
	timer_ = timer_ - dt;
	if (timer_ < 0)
		isAlive_ = false;

	//�A�j���[�V�����X�V
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
			float t = (float)(0.25 * sin(2.0 * DX_PI * (4.0 - timer_) / 2.0f));
			Point bpos = { pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT / 2 };
			DrawCircle(bpos.x, bpos.y, (int)((CHA_WIDTH / 2) + (CHA_WIDTH / 3.5) * t), GetColor(15, 15, 12), TRUE);
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
	