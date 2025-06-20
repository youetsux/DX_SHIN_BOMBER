#include "Stage.h"
#include "./globals.h"
#include <stack>
#include "Item.h"
#include <random>

namespace {
	const float MELTTIMER = 2.0f;//壁が溶けるまでの時間
	const int PROB = 65;//壁が生成される確率

	enum STAGE_TYPE
	{
		NO_BRICK, RANDOM_BRICK, FIXED_BRICK, MAX_STAGE_TYPE
	};
	const STAGE_TYPE stageType = NO_BRICK;
}


//Todo
//壁抜け
//当たり判定
//音
//クリア
//配列のxyを範囲チェックいれる＝アサート防止

void Stage::RefreshStage()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x].type == STAGE_OBJ::BRICK && stageData[y][x].isBreak)
			{
				if(stageData[y][x].meltTimer > 0)
					stageData[y][x].meltTimer -= Time::DeltaTime();
				else
				{
					stageData[y][x].type = STAGE_OBJ::EMPTY;
					stageData[y][x].isBreak = false;
				}

				continue;
			}
		}
	}
}

void Stage::SetRandomBrick()
{
	stageData = vector(STAGE_HEIGHT, vector<StageObj>(STAGE_WIDTH, { STAGE_OBJ::EMPTY, {0,0,0,0}, 0, false, nullptr }));

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (y == 0 || y == STAGE_HEIGHT - 1 || x == 0 || x == STAGE_WIDTH - 1)
			{
				stageData[y][x].type = STAGE_OBJ::WALL;
			}
			else
			{
				if (x % 2 == 0 && y % 2 == 0)
					stageData[y][x].type = STAGE_OBJ::WALL;
				else
				{
					if (GetRand(100) > PROB) {
						stageData[y][x].type = STAGE_OBJ::BRICK;
						stageData[y][x].isBreak = false;
						stageData[y][x].meltTimer = MELTTIMER;

					}
					else
						stageData[y][x].type = STAGE_OBJ::EMPTY;
				}
			}
		}
	}


	stageData[1][1].type = STAGE_OBJ::EMPTY;
	stageData[2][1].type = STAGE_OBJ::EMPTY;
	stageData[3][1].type = STAGE_OBJ::EMPTY;
	stageData[1][2].type = STAGE_OBJ::EMPTY;
	stageData[1][3].type = STAGE_OBJ::EMPTY;

	stageData[STAGE_HEIGHT - 2][STAGE_WIDTH - 2].type = STAGE_OBJ::EMPTY;
	stageData[STAGE_HEIGHT - 3][STAGE_WIDTH - 2].type = STAGE_OBJ::EMPTY;
	stageData[STAGE_HEIGHT - 4][STAGE_WIDTH - 2].type = STAGE_OBJ::EMPTY;
	stageData[STAGE_HEIGHT - 2][STAGE_WIDTH - 3].type = STAGE_OBJ::EMPTY;
	stageData[STAGE_HEIGHT - 2][STAGE_WIDTH - 4].type = STAGE_OBJ::EMPTY;

	setStageRects();
	InitStageItems();
}

void Stage::SetFixedBrick()
{
}

void Stage::SetNoBrick()
{
	stageData = vector(STAGE_HEIGHT, vector<StageObj>(STAGE_WIDTH, { STAGE_OBJ::EMPTY, {0,0,0,0}, 0, false, nullptr }));

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (y == 0 || y == STAGE_HEIGHT - 1 || x == 0 || x == STAGE_WIDTH - 1)
			{
				stageData[y][x].type = STAGE_OBJ::WALL;
			}
			else
			{
				if (x % 2 == 0 && y % 2 == 0)
					stageData[y][x].type = STAGE_OBJ::WALL;
			}
		}
	}

	setStageRects();
	InitRawItems();
}

void Stage::InitRawItems()
{
	for (int k = 0; k < ITEMS::ITEM_MAX; k++)
	{
		for (int i = 0; i < ITEMNUM[k]; i++)
		{
			Point p = { GetRand(STAGE_WIDTH - 1), GetRand(STAGE_HEIGHT - 1) };
			while(stageData[p.y][p.x].type != STAGE_OBJ::EMPTY)
				p = { GetRand(STAGE_WIDTH - 1), GetRand(STAGE_HEIGHT - 1) };
			stageData[p.y][p.x].item = new Item({ p.x * CHA_WIDTH, p.y * CHA_HEIGHT }, (ITEMS)k);
			stageData[p.y][p.x].item->Exposure();
		}
	}
}



void Stage::InitStageItems()
{
	vector<Point> brrickList;
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x].type == STAGE_OBJ::BRICK)
			{
				brrickList.push_back({ x,y });
			}
		}
	}
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(brrickList.begin(), brrickList.end(), engine);

	for (int k = 0; k < ITEMS::ITEM_MAX; k++)
	{
		for (int i = 0; i < ITEMNUM[k]; i++)
		{
			Point& p = brrickList.back();
			stageData[p.y][p.x].item = new Item({ p.x * CHA_WIDTH, p.y * CHA_HEIGHT }, (ITEMS)k);
			brrickList.pop_back();
		}
	}

}

Stage::Stage()
{
	switch (stageType)
	{
	case NO_BRICK:
		SetNoBrick();
		break;
	case RANDOM_BRICK:
		SetRandomBrick();
		break;
	default:
		SetNoBrick();
		break;
	}
	//
}

Stage::~Stage()
{
}

void Stage::Update()
{
	RefreshStage();
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			switch (stageData[y][x].type)
			{
			case STAGE_OBJ::BOMB:
			case STAGE_OBJ::EMPTY:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(102, 205, 170), TRUE);
				break;
			case STAGE_OBJ::WALL:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(119, 136, 153), TRUE);
				break;
			case STAGE_OBJ::BRICK:
				DrawBrick({x,y, CHA_WIDTH, CHA_HEIGHT});
				break;
			default:
				break;
			}
		}
	}
}

void Stage::setStageRects()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x].type == STAGE_OBJ::WALL || stageData[y][x].type == STAGE_OBJ::BRICK)
			{
				stageData[y][x].rect = { x * CHA_WIDTH, y * CHA_HEIGHT,  CHA_WIDTH, CHA_HEIGHT };
			}
		}
	}
}

bool Stage::isBombHere(Rect rec)
{
	int x = rec.x / CHA_WIDTH;
	int y = rec.y / CHA_HEIGHT;
	bool ret = stageData[y][x].type == STAGE_OBJ::BOMB;

	if (ret)
		return true;
	else
		return false;
}


void Stage::DrawBrick(Rect rect)
{
	if (stageData[rect.y][rect.x].isBreak == true && stageData[rect.y][rect.x].meltTimer > 0)
	{
		int col = 109 + (int)((255 - 109) * (1 - stageData[rect.y][rect.x].meltTimer / MELTTIMER));
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(col, 126, 143), TRUE);
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(0, 0, 0), FALSE);
	}
	else {
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(109, 126, 143), TRUE);
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(0, 0, 0), FALSE);
	}
	for (int i = 1; i <= 2; i++)
	{
		DrawLine(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT + i * (CHA_HEIGHT / 3), rect.x * 2 * CHA_WIDTH, rect.y * CHA_HEIGHT + i * (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);
	}
	DrawLine((int)((rect.x + 0.5) * CHA_WIDTH), rect.y * CHA_HEIGHT, (int)((rect.x + 0.5) * CHA_WIDTH), rect.y * CHA_HEIGHT + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((int)((rect.x + 0.25) * CHA_WIDTH), (int)((rect.y + 1.0 / 3) * CHA_HEIGHT), (int)((rect.x + 0.25) * CHA_WIDTH), (int)((rect.y + 1 / 3.0f) * CHA_HEIGHT) + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((int)((rect.x + 0.75) * CHA_WIDTH), (int)((rect.y + 1.0 / 3) * CHA_HEIGHT), (int)((rect.x + 0.75) * CHA_WIDTH), (int)((rect.y + 1 / 3.0f) * CHA_HEIGHT) + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((int)((rect.x + 0.5) * CHA_WIDTH),  (int)((rect.y + 2.0 / 3) * CHA_HEIGHT), (int)((rect.x + 0.5) * CHA_WIDTH),  (int)((rect.y + 2 / 3.0f) * CHA_HEIGHT) + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

}