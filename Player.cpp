#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"
#include "ImGui/imgui.h"
#include "bomb.h"
#include "BombFire.h"
#include "Enemy.h"
#include "easefunction.h"

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
	const Pointf nDir[MAXDIR] = { {0,-1},{0,1},{-1,0},{1,0},{0,0} };

	//const float ANIM_INTERVAL = 2.0 + Direct3D::EaseFunc[OutCubic](MAXSPEED - INITSPEED);
	const int frameNum[4] = { 0,1,2,1 };
	const int yTerm[5] = { 3, 0, 1, 2, 0 };
	bool isGraphic = true;
	float tmm = 1.0f;
	const int MAX_ANIM_FRAME = 4;

	const float DEATH_ANIM_FRAME = 3.0f;
	const float DEATH_ANIM_INTERVAL = 0.2f;
}





Player::Player()
	: pos_({ CHA_WIDTH, CHA_HEIGHT }), playerImage_(-1)
{
	playerState_ = PLAYER_STATE::PLAYER_ALIVE;
	numBomb_ = INITBOMB;
	firePower_ = INITFIRE;
	speed_ = INITSPEED;
	usedBomb_ = 0;
	playerImage_ = LoadGraph("Assets/neko2.png");
	animTimer_ = 0.0f;
	animFrame_ = 0;
	deathDir_ = { (GetRand(10000) / 5000.0f) - 1.0f, (GetRand(10000) / 5000.0f) - 1.0f };
	float dist = sqrt(deathDir_.x * deathDir_.x + deathDir_.y * deathDir_.y);
	deathDir_.x = deathDir_.x / dist;
	deathDir_.y = deathDir_.y / dist;
	timeToDeath_ = DEATH_ANIM_FRAME;
}

Player::~Player()
{
	this->DestroyMe();
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

	switch (playerState_)
	{
	case PLAYER_STATE::PLAYER_ALIVE:
		UpdatePlayerAlive();
		break;
	case PLAYER_STATE::PLAYER_DEAD_READY:
		UpdatePlayerDeadReady();
		break;
	case PLAYER_STATE::PLAYER_DEAD:
		UpdatePlayerDead();
		break;
	default:
		break;
	}



	//デバッグ用コマンドｘ３
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


}

void Player::MovePlayer(Point _op, DIR _inputDir, float _dt)
{
	Pointf moveDist = { speed_ * nDir[_inputDir].x * _dt , speed_ * nDir[_inputDir].y * _dt };

	pos_.x = pos_.x + moveDist.x;
	pos_.y = pos_.y + moveDist.y;

	Stage* stage = (Stage*)FindGameObject<Stage>();
	vector<vector<StageObj>>& stageData = stage->GetStageGrid();
	Rect playerRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };

	bool nb[9]{ false };
	int knum = 0;
	for (int i = 1; i < NEIGHBOURS; i++)
	{
		int x = _op.x / CHA_WIDTH + nineNeibor[i].x;
		int y = _op.y / CHA_HEIGHT + nineNeibor[i].y;
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
	//char nbc[9] = { ' ' };
	//for (int i = 0; i < 9; i++)
	//	if (nb[i])
	//		nbc[i] = 'X';
	//	else
	//		nbc[i] = 'O';



	ImGui::Begin("Enemy");
	//ImGui::Text("%c%c%c", nbc[6], nbc[5], nbc[7]);
	//ImGui::Text("%c%c%c", nbc[4], nbc[0], nbc[1]);
	//ImGui::Text("%c%c%c", nbc[8], nbc[2], nbc[3]);
	//ImGui::Text("%d", knum);
	ImGui::Text("BOM  :%02d", numBomb_);
	ImGui::Text("FIRE :%02d", firePower_);
	ImGui::Text("SPEED:%lf", speed_ * _dt);

	ImGui::End();

	for (int i = 1; i < NEIGHBOURS; i++)
	{
		int x = _op.x / CHA_WIDTH + nineNeibor[i].x;
		int y = _op.y / CHA_HEIGHT + nineNeibor[i].y;
		CheckBoundary(x, y); //範囲外の場合は補正
		StageObj& obj = stageData[y][x];
		obj.rect = { x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };

		if (obj.type == STAGE_OBJ::EMPTY) continue;

		if (obj.type == STAGE_OBJ::WALL || obj.type == STAGE_OBJ::BRICK || obj.type == STAGE_OBJ::BOMB)
		{

			if (CheckHit(playerRect, obj.rect))
			{
				if (_inputDir == LEFT || _inputDir == RIGHT) {
					if (playerRect.x < obj.rect.x + playerRect.w && playerRect.x + playerRect.w > obj.rect.x) {
						pos_.x = (x - nineNeibor[i].x) * CHA_WIDTH;
					}
					if (knum < 2)//接している壁の数が2つ以上あったら壁ズリしない
					{
						if (playerRect.GetCenter().y > obj.rect.GetCenter().y) {
							pos_.y = pos_.y + speed_ * _dt;
							if (pos_.y > obj.rect.y + CHA_HEIGHT)
								pos_.y = obj.rect.y + CHA_HEIGHT;
						}
						else if (playerRect.GetCenter().y < obj.rect.GetCenter().y) {
							pos_.y = pos_.y - speed_ * _dt;
							if (pos_.y + CHA_HEIGHT < obj.rect.y)
								pos_.y = obj.rect.y - CHA_HEIGHT;
						}
					}

				}
				if (_inputDir == UP || _inputDir == DOWN) {
					if (playerRect.y < obj.rect.y + playerRect.h && playerRect.y + playerRect.h > obj.rect.y) {
						pos_.y = (y - nineNeibor[i].y) * CHA_HEIGHT;
					}
					if (knum < 2)//接している壁の数が2つ以上あったら壁ズリしない
					{
						if (playerRect.GetCenter().x > obj.rect.GetCenter().x) {
							pos_.x = pos_.x + speed_ * _dt;
							if (pos_.x > obj.rect.x + CHA_HEIGHT)
								pos_.x = obj.rect.x + CHA_HEIGHT;
						}
						else if (playerRect.GetCenter().x < obj.rect.GetCenter().x) {
							pos_.x = pos_.x - speed_ * _dt;
							if (pos_.x + CHA_HEIGHT < obj.rect.x)
								pos_.x = obj.rect.x - CHA_HEIGHT;
						}
					}
				}

			}
		}
	}

}

void Player::UpdatePlayerAlive()
{
	int ox = (int)pos_.x;
	int oy = (int)pos_.y;

	GetInputDir();

	float dt = tmm * Time::DeltaTime();

	MovePlayer({ ox, oy }, inputDir_, dt);
	//爆弾と自分の座標が同じときはスルーする

	//player vs Items;
	PlayerVSItem();
	//player vs Enemy;
	PlayerVSEnemy();
	//player vs BombFire
	PlayerVSBombFire();

	//現在使用済みの爆弾の数を計算
	std::list<Bomb*> bombs = FindGameObjects<Bomb>();
	usedBomb_ = (int)bombs.size();

	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		//ボムの設置位置をいい感じで整数化
		Point bpos = { CHA_WIDTH * (((int)pos_.x + CHA_WIDTH / 2) / CHA_WIDTH),CHA_HEIGHT * (((int)pos_.y + CHA_HEIGHT / 2) / CHA_HEIGHT) };
		PutBomb(bpos);
	}

	float val = (speed_ - INITSPEED) / (MAXSPEED - INITSPEED);
	float ANIM_INTERVAL = 0.2f - 0.1 * Direct3D::EaseFunc["OutCubic"](val);

	//アニメーション更新
	animTimer_ += tmm * Time::DeltaTime();
	if (animTimer_ > ANIM_INTERVAL)
	{
		animFrame_ = (animFrame_ + 1) % MAX_ANIM_FRAME;
		animTimer_ = animTimer_ - ANIM_INTERVAL;
	}
}

void Player::UpdatePlayerDeadReady()
{

	float ANIM_INTERVAL = 0.2f;
	float FLY_SPEED = 800.0f;

	timeToDeath_ -= tmm * Time::DeltaTime();

	pos_.x = pos_.x + deathDir_.x * FLY_SPEED * Time::DeltaTime();
	pos_.y = pos_.y + deathDir_.y * FLY_SPEED * Time::DeltaTime();
	if (timeToDeath_ < 0) {
		timeToDeath_ = DEATH_ANIM_FRAME;
		SceneManager::ChangeScene("GAMEOVER");
	}
}

void Player::UpdatePlayerDead()
{
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
		float dist = CalcDistance(itmCenter, playerCenter);
		if (dist < COLLISION_DIST * CHA_WIDTH)
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
	std::list<Enemy*> Enemies = FindGameObjects<Enemy>();
	for (auto& enemy : Enemies)
	{
		Rect eRect = { enemy->GetPos(), CHA_WIDTH, CHA_HEIGHT };
		Rect pRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
		Point eCenter = eRect.GetCenter();
		Point playerCenter = pRect.GetCenter();
		//float dist = (float)((eCenter.x - playerCenter.x) * (eCenter.x - playerCenter.x) + (eCenter.y - playerCenter.y) * (eCenter.y - playerCenter.y));
		float dist = CalcDistance(eCenter, playerCenter);
		if (dist < COLLISION_DIST * CHA_WIDTH)
		{
			//当たり判定表示用
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox((int)pos_.x, (int)pos_.y, (int)pos_.x + CHA_WIDTH, (int)pos_.y + CHA_HEIGHT, GetColor(0, 200, 200), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			
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
			//float dist = (fc.x - playerCenter.x) * (fc.x - playerCenter.x) + (fc.y - playerCenter.y) * (fc.y - playerCenter.y);
			float dist = CalcDistance(fc, playerCenter);
			if (dist < COLLISION_DIST * CHA_WIDTH)
			{
				//当たり判定表示用
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(0, 200, 200), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				//SceneManager::ChangeScene("GAMEOVER");
				playerState_ = PLAYER_STATE::PLAYER_DEAD_READY;
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
		if (playerState_ ==  PLAYER_STATE::PLAYER_DEAD_READY) {
			//int hGraph = MakeGraph(CHA_WIDTH, CHA_HEIGHT, TRUE);
			//SetDrawScreen(hGraph);     // 回転グラフィックに描く
			//ClearDrawScreen();               // クリア（前の回転状態を消す）

			// 回転描画（中心を軸に）
			DrawRectRotaGraph(pos_.x, pos_.y, frameNum[animFrame_] * 32, (4 + yTerm[inputDir_]) * 32, 32, 32, (float)CHA_WIDTH / 32.0, timeToDeath_*10.0, playerImage_, TRUE);
			
			//DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, frameNum[animFrame_] * 32, (4 + yTerm[inputDir_]) * 32, 32, 32, playerImage_, TRUE);
		
		}
		else
			DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, frameNum[animFrame_] * 32, (4 + yTerm[inputDir_]) * 32, 32, 32, playerImage_, TRUE);
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
		speed_ += CHA_WIDTH / 3;
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