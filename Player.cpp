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
	int ox = pos_.x, oy = pos_.y;

	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		pos_.y--;
		inputDir = UP;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		pos_.y++;
		inputDir = DOWN;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		pos_.x--;
		inputDir = LEFT;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		pos_.x++;
		inputDir = RIGHT;
	}

	Stage* stage = (Stage*)FindGameObject<Stage>();
	vector<vector<StageObj>>& stageData = stage->GetStageGrid();
	Rect playerRect = { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };

	//for (auto& obj : stage->GetStageGrid())
	//{
	//	for (auto& itr : obj) {
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			StageObj& obj = stageData[y][x];
			if (obj.type == STAGE_OBJ::EMPTY) continue;
			if (CheckHit(playerRect, obj.rect))
			{
				Rect tmpRectX = { ox, pos_.y, CHA_WIDTH, CHA_HEIGHT };
				Rect tmpRecty = { pos_.x, oy, CHA_WIDTH, CHA_HEIGHT };
				//xŽ²•ûŒü‚Åˆø‚ÁŠ|‚©‚Á‚½
				if (!CheckHit(tmpRectX, obj.rect))
				{
					pos_.x = ox;//xŽ²•ûŒü‚É‚ß‚èž‚ÝC³
					//•ÇƒYƒŠ
					Point centerMe = Rect{ pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.y > centerObj.y)
					{
						pos_.y++;
					}
					else if (centerMe.y < centerObj.y)
					{
						pos_.y--;
					}
				}
				else if (!CheckHit(tmpRecty, obj.rect))
				{
					pos_.y = oy;//y•ûŒü‚Éˆø‚Á‚©‚©‚Á‚½‚ç‚ß‚èž‚ÝC³
					//•ÇƒYƒŠ
					Point centerMe = Rect{ pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.x > centerObj.x)
					{
						pos_.x++;
					}
					else if (centerMe.x < centerObj.x)
					{
						pos_.x--;
					}
				}
				else
				{
					//‚»‚êˆÈŠO‚Åˆø‚ÁŠ|‚©‚Á‚½Žži‚¢‚Â‚â‚ñj=‚Ê‚é‚Á‚Æ“®‚­ŽžˆÈŠO‚Å‚ß‚èž‚ñ‚¾Žž
					//–³‚¢‚ÆŽv‚Á‚Ä‚½‚¯‚Ç—L‚Á‚½‚Ë‚¥
					pos_.x = ox;
					pos_.y = oy;
				}
			}
		}
	}


	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		//if (maxBomb_ - usedBomb_ > 0) {
		Point bpos = { CHA_WIDTH * ((pos_.x + CHA_WIDTH / 2) / CHA_WIDTH),CHA_HEIGHT * ((pos_.y + CHA_HEIGHT / 2) / CHA_HEIGHT) };
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
