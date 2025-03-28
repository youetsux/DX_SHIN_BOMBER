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

	//BOM,FIRE,SPEED
	const int ITEMNUM[ITEM_MAX]{ 4, 4, 3 };

}

class Stage :
    public GameObject
{
	vector<vector<StageObj>> stageData;
	void DrawBrick(Rect rect);
	void RefreshStage();//仮
	void SetRandomBrick();
	void SetFixedBrick();
	void SetNoBrick();
	void InitRawItems();//むき出しのアイテムを設置
	void InitStageItems();//ブロック内にアイテムを設置
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	StageObj GetStageData(int x, int y) {return stageData[y][x];}
	vector<vector<StageObj>>& GetStageGrid() { return stageData; }
	void setStageRects();
	bool isBombHere(Rect rec);

};

