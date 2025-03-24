#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"
#include "ImGui/imgui.h"
#include "bomb.h"

namespace {
	const float SPEED = 100.0f;
	DIR inputDir = NONE;
	const int MAXBOMBS = 5;
	const int MAXFIRE = 3;
	//const Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
	
	const int NEIGHBOURS = 9;
	const Point nineNeibor[NEIGHBOURS] = { {0,0}, {1,0}, {0,1}, {1,1}, {-1,0}, {0,-1}, {-1,-1}, {1,-1}, {-1,1} };


}





Player::Player()
	: pos_({ CHA_WIDTH, CHA_HEIGHT }), playerImage_(-1)
{
	maxBomb_ = MAXBOMBS;
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
		inputDir = UP;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		inputDir = DOWN;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		inputDir = LEFT;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
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

	//Ç±ÇÍÇ‡é©ï™ÇÃÇWãﬂñTÇæÇØÇ‚ÇÍÇŒÇ¢Ç¢ÇÃÇ≈ÇÕÅI
	//ìñÇΩÇ¡ÇΩï«ÇÃç¿ïWÇ™ÇÌÇ©ÇÍÇŒï‚ê≥à íuÇÕÇÌÇ©ÇÈÇÕÇ∏ÇæÇ©ÇÁåvéZÇµÇøÇ‹Ç¶ÇŒÇ¢Ç¢Ç∂Ç·ÇÒ
	//for (int y = 0; y < STAGE_HEIGHT; y++)
	//{
	//	for (int x = 0; x < STAGE_WIDTH; x++)
	//	{
	for (int i = 0; i < NEIGHBOURS; i++)
	{
		int x = ox / CHA_WIDTH + nineNeibor[i].x;
		int y = oy / CHA_HEIGHT + nineNeibor[i].y;
		StageObj& obj = stageData[y][x];
		obj.rect = { x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT };

		if (obj.type == STAGE_OBJ::EMPTY) continue;
		
		if (obj.type == STAGE_OBJ::WALL || obj.type == STAGE_OBJ::BRICK || obj.type == STAGE_OBJ::BOMB)
		{
			if (CheckHit(playerRect, obj.rect))
			{
				Rect tmpRectX = { ox, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT };
				Rect tmpRecty = { (int)pos_.x, oy, CHA_WIDTH, CHA_HEIGHT };
				//xé≤ï˚å¸Ç≈à¯Ç¡ä|Ç©Ç¡ÇΩ
				if (!CheckHit(tmpRectX, obj.rect))
				{
					pos_.x = ox;//xé≤ï˚å¸Ç…ÇﬂÇËçûÇ›èCê≥
					//ï«ÉYÉä
					Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.y > centerObj.y)
					{
						pos_.y = pos_.y + SPEED * dt;
					}
					else if (centerMe.y < centerObj.y)
					{
						pos_.y = pos_.y - SPEED * dt;
					}
				}
				if (!CheckHit(tmpRecty, obj.rect))
				{
					pos_.y = oy;//yï˚å¸Ç…à¯Ç¡Ç©Ç©Ç¡ÇΩÇÁÇﬂÇËçûÇ›èCê≥
					//ï«ÉYÉä
					Point centerMe = Rect{ (int)pos_.x, (int)pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
					Point centerObj = obj.rect.GetCenter();
					if (centerMe.x > centerObj.x)
					{
						//pos_.x++;
						pos_.x = pos_.x + SPEED * dt;
					}
					else if (centerMe.x < centerObj.x)
					{
						//pos_.x--;
						pos_.x = pos_.x - SPEED * dt;
					}
				}
			}
		}

		//	}
		//}
	}

	std::list<Bomb*> bombs = FindGameObjects<Bomb>();
	usedBomb_ = bombs.size();

	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		Point bpos = { CHA_WIDTH * (((int)pos_.x + CHA_WIDTH / 2) / CHA_WIDTH),CHA_HEIGHT * (((int)pos_.y + CHA_HEIGHT / 2) / CHA_HEIGHT) };
		PutBomb(bpos);
	}
}


void Player::PutBomb(const Point& pos)
{
	std::list<Bomb*> bombs = FindGameObjects<Bomb>();
	if (maxBomb_ - usedBomb_ > 0) {
		for (auto& b : bombs) {
			if (b->GetPos().x == pos.x && b->GetPos().y == pos.y)
				return;
		}
		new Bomb(pos, MAXFIRE);//å„Ç≈ïœêîÇ…ïœÇ¶ÇÈ
	}
}

void Player::Draw()
{

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
