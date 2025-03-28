#pragma once


class Item;

struct Point
{
	int x, y;
};

struct Pointf
{
	float x, y;
};

//これ絶対マップ＋フラグ式にしたほうが、スッキリするぞ。。。
struct Rect
{
	int x, y, w, h;
	Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
	Rect(Point p, int w, int h) : x(p.x), y(p.y), w(w), h(h) {}
	Point GetCenter() { return Point{ x + w / 2, y + h / 2 }; }
};


enum STAGE_OBJ {
	EMPTY,
	WALL,
	BRICK,
	BOMB,
	BOMBFIRE,
	MAX_OBJECT
};

//マップデータ生成用
struct StageObj
{
	STAGE_OBJ type;
	Rect rect;
	float meltTimer;
	bool isBreak;
	Item* item;
};

struct StageRect
{
	STAGE_OBJ type;
	Rect rect;
	float meltTimer;
	bool isBreak;
	bool isStatic;
};

enum DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE,
	MAXDIR
};

enum ITEMS
{
	ITEM_BOMB,
	ITEM_FIRE,
	ITEM_SPEED,
	ITEM_MAX
};

const int CHA_WIDTH = 48;
const int CHA_HEIGHT = 48;

constexpr int STAGE_WIDTH = 25;
constexpr int STAGE_HEIGHT = 15;

//const int STAGE_WIDTH = 15;
//const int STAGE_HEIGHT = 15;


inline bool CheckHit(const Rect& me, const Rect& other)
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

inline void CheckBoundary(int& x, int& y)
{
	//return x >= 0 && x < STAGE_WIDTH && y >= 0 && y < STAGE_HEIGHT;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= STAGE_WIDTH) x = STAGE_WIDTH - 1;
	if (y >= STAGE_HEIGHT) y = STAGE_HEIGHT - 1;
}
