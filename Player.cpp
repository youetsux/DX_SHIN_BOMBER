#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"
#include "ImGui/imgui.h"
#include "bomb.h"
#include "BombFire.h"
#include "Enemy.h"

namespace {
	//DIR inputDir = NONE;

	const int MAXBOMBS = 10;
	const int MAXFIRE = 8;
	const float MAXSPEED = 200.0f;
	//const Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
	const int INITBOMB = 1;
	const int INITFIRE = 1;
	const float INITSPEED = 80;

	const int NEIGHBOURS = 9;
	const Point nineNeibor[NEIGHBOURS] = { {0,0}, {1,0}, {0,1}, {1,1}, {-1,0}, {0,-1}, {-1,-1}, {1,-1}, {-1,1} };

	const float ANIM_INTERVAL = 0.2f;
	const int frameNum[4] = { 0,1,2,1 };
	const int yTerm[5] = {  3, 0, 1, 2, 0 };
	bool isGraphic = true;
}





Player::Player()
	: pos_({ CHA_WIDTH, CHA_HEIGHT }), playerImage_(-1)
{
	numBomb_ = INITBOMB;
	firePower_ = INITFIRE;
	speed_ = INITSPEED;
	usedBomb_ = 0;
	playerImage_ = LoadGraph("Assets/neko2.png");
	animTimer_ = 0.0f;
	animFrame_ = 0;
}

Player::~Player()
{
}

void Player::GetInputDir()
{
	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		inputDir_ = UP;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		inputDir_ = DOWN;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		inputDir_ = LEFT;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		inputDir_ = RIGHT;
	}
	else
	{
		inputDir_ = NONE;
	}
}


void Player::Update()
{

	int ox = (int)pos_.x;
	int oy = (int)pos_.y;

	GetInputDir();

	Pointf nDir[MAXDIR] = { {0,-1},{0,1},{-1,0},{1,0},{0,0} };
	float dt = Time::DeltaTime();
	Pointf moveDist = { speed_ * nDir[inputDir_].x * dt , speed_ * nDir[inputDir_].y * dt };

	pos_.x = pos_.x + moveDist.x;
	pos_.y = pos_.y + moveDist.y;

	Stage* stage = (Stage*)FindGameObject<Stage>();
	vector<vector<StageObj>>& stageData = stage->GetStageGrid();
	Rect playerRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };

	bool nb[9]{ false };
	int knum = 0;
	for (int i = 1; i < NEIGHBOURS; i++)
	{
		int x = ox / CHA_WIDTH + nineNeibor[i].x;
		int y = oy / CHA_HEIGHT + nineNeibor[i].y;
		CheckBoundary(x, y); //範囲外の場合は補正
		StageObj& obj = stageData[y][x];
		obj.rect = { x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };

		if (obj.type == STAGE_OBJ::EMPTY) continue;

		if (obj.type == STAGE_OBJ::WALL || obj.type == STAGE_OBJ::BRICK || obj.type == STAGE_OBJ::BOMB)
		{
			if (CheckHit(playerRect, obj.rect))
			{
				nb[i] = true;
				knum++;
			}
		}
	}

	//const Point nineNeibor[NEIGHBOURS] = { {0,0}, {1,0}, {0,1}, {1,1}, {-1,0}, {0,-1}, {-1,-1}, {1,-1}, {-1,1} };
	char nbc[9] = { ' ' };
	for (int i = 0; i < 9; i++)
		if (nb[i])
			nbc[i] = 'X';
		else
			nbc[i] = 'O';



	ImGui::Begin("Enemy");
	ImGui::Text("%c%c%c", nbc[6], nbc[5], nbc[7]);
	ImGui::Text("%c%c%c", nbc[4], nbc[0], nbc[1]);
	ImGui::Text("%c%c%c", nbc[8], nbc[2], nbc[3]);
	ImGui::Text("%d", knum);
	ImGui::Text("BOM  :%02d", numBomb_);
	ImGui::Text("FIRE :%02d", firePower_);
	ImGui::Text("SPEED:%lf", speed_ * dt);
	ImGui::End();

	for (int i = 1; i < NEIGHBOURS; i++)
	{
		int x = ox / CHA_WIDTH + nineNeibor[i].x;
		int y = oy / CHA_HEIGHT + nineNeibor[i].y;
		CheckBoundary(x, y); //範囲外の場合は補正
		StageObj& obj = stageData[y][x];
		obj.rect = { x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };

		if (obj.type == STAGE_OBJ::EMPTY) continue;

		if (obj.type == STAGE_OBJ::WALL || obj.type == STAGE_OBJ::BRICK || obj.type == STAGE_OBJ::BOMB)
		{

			if (CheckHit(playerRect, obj.rect))
			{
				if (inputDir_ == LEFT || inputDir_ == RIGHT) {
					if (playerRect.x < obj.rect.x + playerRect.w && playerRect.x + playerRect.w > obj.rect.x) {
						pos_.x = (x - nineNeibor[i].x) * CHA_WIDTH;
					}
					if (knum < 2)
					{
						if (playerRect.GetCenter().y > obj.rect.GetCenter().y) {
							pos_.y = pos_.y + speed_ * dt;
							if (pos_.y > obj.rect.y + CHA_HEIGHT)
								pos_.y = obj.rect.y + CHA_HEIGHT;
						}
						else if (playerRect.GetCenter().y < obj.rect.GetCenter().y) {
							pos_.y = pos_.y - speed_ * dt;
							if (pos_.y + CHA_HEIGHT < obj.rect.y)
								pos_.y = obj.rect.y - CHA_HEIGHT;
						}
					}

				}
				if (inputDir_ == UP || inputDir_ == DOWN) {
					if (playerRect.y < obj.rect.y + playerRect.h && playerRect.y + playerRect.h > obj.rect.y) {
						pos_.y = (y - nineNeibor[i].y) * CHA_HEIGHT;
					}
					if (knum < 2)
					{
						if (playerRect.GetCenter().x > obj.rect.GetCenter().x) {
							pos_.x = pos_.x + speed_ * dt;
							if (pos_.x > obj.rect.x + CHA_HEIGHT)
								pos_.x = obj.rect.x + CHA_HEIGHT;
						}
						else if (playerRect.GetCenter().x < obj.rect.GetCenter().x) {
							pos_.x = pos_.x - speed_ * dt;
							if (pos_.x + CHA_HEIGHT < obj.rect.x)
								pos_.x = obj.rect.x - CHA_HEIGHT;
						}
					}
				}

			}
		}
	}
	//爆弾と自分の座標が同じときはスルーする

	//player vs Items;
	PlayerVSItem();
	PlayerVSEnemy();
	PlayerVSBombFire();

	std::list<Bomb*> bombs = FindGameObjects<Bomb>();
	usedBomb_ = (int)bombs.size();

	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		Point bpos = { CHA_WIDTH * (((int)pos_.x + CHA_WIDTH / 2) / CHA_WIDTH),CHA_HEIGHT * (((int)pos_.y + CHA_HEIGHT / 2) / CHA_HEIGHT) };
		PutBomb(bpos);
	}

	//デバッグ用コマンド
	if (Input::IsKeyDown(KEY_INPUT_F1))
	{
		FireUP();
	}
	if (Input::IsKeyDown(KEY_INPUT_F2))
	{
		SpeedUP();
	}
	if (Input::IsKeyDown(KEY_INPUT_F3))
	{
		BombUP();
	}
	//アニメーション更新
	animTimer_ += Time::DeltaTime();
	if (animTimer_ > ANIM_INTERVAL)
	{
		animFrame_ = (animFrame_ + 1) % 4;
		animTimer_ = animTimer_ - ANIM_INTERVAL;
	}
}

void Player::PlayerVSItem()
{
	const float COLLISION_DIST = 0.7f;
	//player vs Items;
	std::list<Item*> Items = FindGameObjects<Item>();
	for (auto& itm : Items)
	{
		Rect itmRect = { itm->GetPos(), CHA_WIDTH, CHA_HEIGHT };
		Rect pRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
		Point itmCenter = itmRect.GetCenter();
		Point playerCenter = pRect.GetCenter();
		float dist = (float)((itmCenter.x - playerCenter.x) * (itmCenter.x - playerCenter.x) + (itmCenter.y - playerCenter.y) * (itmCenter.y - playerCenter.y));
		if (sqrt(dist) < COLLISION_DIST * CHA_WIDTH)
		{
			ITEMS kind = itm->UseItem();
			switch (kind)
			{
			case ITEMS::ITEM_BOMB:
				BombUP();
				break;
			case ITEMS::ITEM_FIRE:
				FireUP();
				break;
			case ITEMS::ITEM_SPEED:
				SpeedUP();
				break;
			default:
				break;
			}
		}
	}
}

void Player::PlayerVSEnemy()
{
	//player vs Items;
	const float COLLISION_DIST = 0.6f;
	std::list<Enemy *> Enemies = FindGameObjects<Enemy>();
	for (auto& enemy : Enemies)
	{
		Rect eRect = { enemy->GetPos(), CHA_WIDTH, CHA_HEIGHT };
		Rect pRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
		Point eCenter = eRect.GetCenter();
		Point playerCenter = pRect.GetCenter();
		float dist = (float)((eCenter.x - playerCenter.x) * (eCenter.x - playerCenter.x) + (eCenter.y - playerCenter.y) * (eCenter.y - playerCenter.y));
		if (sqrt(dist) < COLLISION_DIST * CHA_WIDTH)
		{
			//当たり判定表示用
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox((int)pos_.x, (int)pos_.y, (int)pos_.x + CHA_WIDTH, (int)pos_.y + CHA_HEIGHT, GetColor(0, 200, 200), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SceneManager::ChangeScene("GAMEOVER");
		}
	}
}

void Player::PlayerVSBombFire()
{
	const float COLLISION_DIST = 0.6f;

	std::list<BombFire*> bfList = FindGameObjects<BombFire>();

	for (auto& itr : bfList)
	{
		std::vector<BomRect>& bRects = itr->GetBomRectList();
		for (auto& itrRec : bRects)
		{
			Point fc = itrRec.rect.GetCenter();	
			Rect pRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };

			Point playerCenter = pRect.GetCenter();
			float dist = (fc.x - playerCenter.x) * (fc.x - playerCenter.x) + (fc.y - playerCenter.y) * (fc.y - playerCenter.y);
			if (sqrt(dist) < COLLISION_DIST * CHA_WIDTH)
			{
				//当たり判定表示用
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(0, 200, 200), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				SceneManager::ChangeScene("GAMEOVER");
			}

		}

	}


}



void Player::PutBomb(const Point& pos)
{
	std::list<Bomb*> bombs = FindGameObjects<Bomb>();
	if (numBomb_ - usedBomb_ > 0) {
		for (auto& b : bombs) {
			if (b->GetPos().x == pos.x && b->GetPos().y == pos.y)
				return;
		}
		new Bomb(pos, firePower_);//後で変数に変える
	}
}



void Player::Draw()
{
	if (isGraphic)
	{
		DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, frameNum[animFrame_] * 32, (4 + yTerm[inputDir_])*32, 32, 32, playerImage_, TRUE);
	}
	else
		DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(255, 10, 10), TRUE);
}

bool Player::CheckHit(const Rect& me, const Rect& other)
{
	if (me.x < other.x + other.w &&
		me.x + me.w > other.x &&
		me.y < other.y + other.h &&
		me.y + me.h > other.y)
	{
		//当たり判定表示用
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawBox(other.x, other.y, other.x + CHA_WIDTH, other.y + CHA_HEIGHT, GetColor(0, 200, 200), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		return true;
	}
	return false;
}

void Player::FireUP()
{
	if (firePower_ < MAXFIRE)
	{
		firePower_++;
	}

}

void Player::SpeedUP()
{
	if (speed_ < MAXSPEED)
	{
		speed_ += CHA_WIDTH/3;
	}
}

void Player::BombUP()
{
	if (numBomb_ < MAXBOMBS)
	{
		numBomb_++;
	}
}



//for (int i = 1; i < NEIGHBOURS; i++)
//{
//	int x = ox / CHA_WIDTH + nineNeibor[i].x;
//	int y = oy / CHA_HEIGHT + nineNeibor[i].y;
//	CheckBoundary(x, y); //範囲外の場合は補正
//	StageObj& obj = stageData[y][x];
//	obj.rect = { x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };
//	if (obj.type == STAGE_OBJ::EMPTY) continue;
//	if (obj.type == STAGE_OBJ::WALL || obj.type == STAGE_OBJ::BRICK || obj.type == STAGE_OBJ::BOMB)
//	{
//		if (CheckHit(playerRect, obj.rect))
//		{
//			Rect tmpRectX = { ox, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
//			Rect tmpRecty = { (int)pos_.x, oy, CHA_WIDTH, CHA_HEIGHT };
//			//x軸方向で引っ掛かった
//			if (!CheckHit(tmpRectX, obj.rect))
//			{
//				pos_.x = (int)ox;//x軸方向にめり込み修正
//				//壁ズリ
//				Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
//				Point centerObj = obj.rect.GetCenter();
//				if (centerMe.y > centerObj.y)
//				{
//					pos_.y = pos_.y + speed_ * dt;
//				}
//				else if (centerMe.y < centerObj.y)
//				{
//					pos_.y = pos_.y - speed_ * dt;
//				}
//			}
//			else if (!CheckHit(tmpRecty, obj.rect))
//			{
//				pos_.y = (int)oy;//y方向に引っかかったらめり込み修正
//				//壁ズリ
//				Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
//				Point centerObj = obj.rect.GetCenter();
//				if (centerMe.x > centerObj.x)
//				{
//					pos_.x = pos_.x + speed_ * dt;
//				}
//				else if (centerMe.x < centerObj.x)
//				{
//					pos_.x = pos_.x - speed_ * dt;
//				}
//			}