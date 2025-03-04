#pragma once
#include "./Library/GameObject.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "./Source/Screen.h"
#include "./globals.h"

using std::vector;


constexpr int STAGE_WIDTH = 31;
constexpr int STAGE_HEIGHT =21;


class Stage :
    public GameObject
{
	vector<vector<StageObj>> stageData;
	vector<Rect> stageRects;
	void DrawBrick(Rect rect);
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	StageObj GetStageData(int x, int y) {return stageData[y][x];}
	vector<Rect> GetStageRects() { return stageRects; }
	vector<vector<StageObj>>& GetStageGrid() { return stageData; }
	void setStageRects();
};

