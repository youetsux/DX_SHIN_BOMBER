#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"
#include "ImGui/imgui.h"
#include "bomb.h"

namespace {
	//enum DIR
	//{
	//	UP,
	//	DOWN,
	//	LEFT,
	//	RIGHT,
	//	NONE,
	//	MAXDIR
	//};
	DIR inputDir = NONE;
}


Player::Player()
	: pos_({ CHA_WIDTH, CHA_HEIGHT }), playerImage_(-1)
{
	maxBomb_ = 5;
	usedBomb_ = 0;
}

Player::~Player()
{
}

void Player::Update()
{
	float SPEED = 100.0f;
	int ox = pos_.x, oy = pos_.y;

	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		//pos_.y--;
		inputDir = UP;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		//pos_.y++;
		inputDir = DOWN;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		//pos_.x--;
		inputDir = LEFT;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		//pos_.x++;
		inputDir = RIGHT;
	}
	else
	{
		inputDir = NONE;
	}

	Pointf nDir[MAXDIR] = { {0,-1},{0,1},{-1,0},{1,0},{0,0} };
	float dt = Time::DeltaTime();
	pos_.x = pos_.x + SPEED * nDir[inputDir].x * dt;
	pos_.y = pos_.y + SPEED * nDir[inputDir].y * dt;


	Stage* stage = (Stage*)FindGameObject<Stage>();
	vector<vector<StageObj>>& stageData = stage->GetStageGrid();
	Rect playerRect = { (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };


	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			StageObj& obj = stageData[y][x];
			if (obj.type == STAGE_OBJ::EMPTY) continue;
			if (CheckHit(playerRect, obj.rect))
			{
				Rect tmpRectX = { ox, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
				Rect tmpRecty = { (int)pos_.x, oy, CHA_WIDTH, CHA_HEIGHT };
				//xŽ²•ûŒü‚Åˆø‚ÁŠ|‚©‚Á‚½
				if (!CheckHit(tmpRectX, obj.rect))
				{
					pos_.x = ox;//xŽ²•ûŒü‚É‚ß‚èž‚ÝC³
					//•ÇƒYƒŠ
					Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.y > centerObj.y)
					{
						//pos_.y++;
						pos_.y = pos_.y + SPEED * dt;
					}
					else if (centerMe.y < centerObj.y)
					{
						//pos_.y--;
						pos_.y = pos_.y - SPEED * dt;
					}
				}
				if (!CheckHit(tmpRecty, obj.rect))
				{
					pos_.y = oy;//y•ûŒü‚Éˆø‚Á‚©‚©‚Á‚½‚ç‚ß‚èž‚ÝC³
					//•ÇƒYƒŠ
					Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.x > centerObj.x)
					{
						//pos_.x++;
						pos_.x = pos_.x + SPEED  * dt;
					}
					else if (centerMe.x < centerObj.x)
					{
						//pos_.x--;
						pos_.x = pos_.x - SPEED  * dt;
					}
				}
			}
		}
	}


	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		//if (maxBomb_ - usedBomb_ > 0) {
		Point bpos = { CHA_WIDTH * (((int)pos_.x + CHA_WIDTH / 2) / CHA_WIDTH),CHA_HEIGHT * (((int)pos_.y + CHA_HEIGHT / 2) / CHA_HEIGHT) };
		new Bomb(bpos, 5);
		//usedBomb_++;
	//}
	}
}

void Player::Draw()
{
	ImGui::Begin("config 1");

	ImGui::End();

	DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(255, 10, 10), TRUE);
}

bool Player::CheckHit(const Rect& me, const Rect& other)
{
	if (me.x < other.x + other.w &&
		me.x + me.w > other.x &&
		me.y < other.y + other.h &&
		me.y + me.h > other.y)
	{
		return true;
	}
	return false;
}
