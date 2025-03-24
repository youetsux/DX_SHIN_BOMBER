#include "Stage.h"
#include "./globals.h"
#include <stack>

namespace {
	float MELTTIMER = 2.0f;//�ǂ��n����܂ł̎���
}




void Stage::DrawBrick(Rect rect)
{
	if (stageData[rect.y][rect.x].isBreak == true && stageData[rect.y][rect.x].meltTimer > 0)
	{
		int col = 109 + (255-109) * (1 - stageData[rect.y][rect.x].meltTimer / MELTTIMER);
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(col, 126, 143), TRUE);
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(0, 0, 0), FALSE);
	}
	else {
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(109, 126, 143), TRUE);
		DrawBox(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT, rect.x * CHA_WIDTH + CHA_WIDTH, rect.y * CHA_HEIGHT + CHA_HEIGHT, GetColor(0, 0, 0), FALSE);
	}
	for (int i = 1; i <= 2; i++)
	{
		DrawLine(rect.x * CHA_WIDTH, rect.y * CHA_HEIGHT+i*(CHA_HEIGHT/3), rect.x * 2 * CHA_WIDTH, rect.y * CHA_HEIGHT + i * (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);
	}
	DrawLine((rect.x + 0.5) * CHA_WIDTH , rect.y * CHA_HEIGHT, (rect.x + 0.5) * CHA_WIDTH, rect.y * CHA_HEIGHT +  (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((rect.x + 0.25) * CHA_WIDTH, (rect.y + 1.0 / 3) * CHA_HEIGHT, (rect.x + 0.25) * CHA_WIDTH, (rect.y + 1 / 3.0f) * CHA_HEIGHT + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((rect.x + 0.75) * CHA_WIDTH, (rect.y + 1.0 / 3) * CHA_HEIGHT, (rect.x + 0.75) * CHA_WIDTH, (rect.y + 1 / 3.0f) * CHA_HEIGHT + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);

	DrawLine((rect.x + 0.5) * CHA_WIDTH, (rect.y+2.0/3) * CHA_HEIGHT, (rect.x + 0.5) * CHA_WIDTH, (rect.y+2/3.0f) * CHA_HEIGHT + (CHA_HEIGHT / 3), GetColor(0, 0, 0), 1);


}

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

Stage::Stage()
{
	stageData = vector(STAGE_HEIGHT, vector<StageObj>(STAGE_WIDTH, { STAGE_OBJ::EMPTY,{0,0,0,0},0,false, false }));

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
					if (GetRand(100) > 70) {
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
	stageData[STAGE_HEIGHT-2][1].type = STAGE_OBJ::EMPTY;
	stageData[STAGE_HEIGHT - 2][STAGE_WIDTH - 2].type = STAGE_OBJ::EMPTY;
	stageData[1][STAGE_WIDTH - 2].type = STAGE_OBJ::EMPTY;
	setStageRects();
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
