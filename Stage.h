#pragma once
#include "./Library/GameObject.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "./Source/Screen.h"
#include "./globals.h"
#include "item.h"

using std::vector;

namespace {
	constexpr int STAGE_WIDTH = 25;
	constexpr int STAGE_HEIGHT = 15;
	//BOM,FIRE,SPEED
	const int ITEMNUM[ITEM_MAX]{ 4, 4, 3 };
}

class Stage :
    public GameObject
{
	vector<vector<StageObj>> stageData;
	void DrawBrick(Rect rect);
	void RefreshStage();//‰¼
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	StageObj GetStageData(int x, int y) {return stageData[y][x];}
	vector<vector<StageObj>>& GetStageGrid() { return stageData; }
	void setStageRects();
	bool isBombHere(Rect rec);
	void InitStageItems();
};

