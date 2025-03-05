#pragma once


	struct Point
	{
		int x, y;
	};

	struct Pointf
	{
		float x, y;
	};

	struct Rect
	{
		int x, y, w, h;
		Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
		Rect(Point p, int w, int h): x(p.x), y(p.y), w(w), h(h) {}
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

	struct StageObj
	{
		STAGE_OBJ obj;
		float weight;
	};

	struct StageRect
	{
		STAGE_OBJ obj;
		Rect rect;
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
	
	const int CHA_WIDTH = 32;
	const int CHA_HEIGHT = 32;

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
