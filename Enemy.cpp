#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include "Player.h"
#include <map>
#include <queue>
#include "Imgui/Imgui.h"

namespace
{
	const Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
	const float SPEED = 100.0f;
	const int NEIGHBOURS = 9;
	const Point nineNeibor[NEIGHBOURS] = { {0,0}, {1,0}, {0,1}, {1,1}, {-1,0}, {0,-1}, {-1,-1}, {1,-1}, {-1,1} };
	//const Point dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

	const float ANIM_INTERVAL = 0.3f;
	const int frameNum[4] = { 0, 1, 2, 1 };
	const int yTerm[5] = { 3, 0, 1, 2, 0 };
	bool isGraphic = true;
}

Enemy::Enemy()
	:pos_({ 0,0 }), isAlive_(true), nextPos_({ 0,0 })
{
	//初期スポーン位置をランダムに設定
	//int rx = 0;
	//int ry = 0;
	//while (rx % 2 == 0 || ry % 2 == 0)
	//{
	//	rx = GetRand(STAGE_WIDTH - 1);
	//	ry = GetRand(STAGE_HEIGHT - 1);
	//}
	
	//敵の初期スポーン位置を設定
	int rx = STAGE_WIDTH - 2;
	int ry = STAGE_HEIGHT - 2;
	pos_ = { (float)rx * CHA_WIDTH, (float)ry * CHA_HEIGHT };
	//敵の初期進行方向を設定
	forward_ = LEFT;
	
	if (isGraphic)
		enemyImage_ = LoadGraph("Assets/kabocha.png");
	//dist = vector(STAGE_HEIGHT, vector<int>(STAGE_WIDTH, INT_MAX));
	//pre = vector(STAGE_HEIGHT, vector<Point>(STAGE_WIDTH, { -1, -1 }));
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	static bool stop = false;

	if (!stop) {
		//移動方向を計算
		Point move = { nDir[forward_].x, nDir[forward_].y };
		//次のフレームの移動位置を計算（予測）
		Pointf npos = { pos_.x + SPEED * move.x * Time::DeltaTime() , pos_.y + SPEED * move.y * Time::DeltaTime() };
		//壁とブロックとあたっているか判定する用
		Point nposI = { (int)npos.x, (int)npos.y };
		Rect nRec = { nposI, CHA_WIDTH, CHA_HEIGHT };
		if (!isHitWall(nRec))//壁とぶつかるならいどうしない
		{
			pos_ = npos;
		}

		//位置のチェックパターンを関数化する

		int prgssx = (int)pos_.x % (CHA_WIDTH);
		int prgssy = (int)pos_.y % (CHA_HEIGHT);

		if (prgssx == 0 && prgssy == 0)
		{
			//チェック座標に到達したら次の方向を指示する
			//次、どっちの方向に行くかここに書く！
			
			//RightHandMove()
			//XYCloserMove();
			//forward_ = DIR::RIGHT;
			forward_ = (DIR)GetRand(3);
		}
	}

	//アニメーション更新
	animTimer_ += Time::DeltaTime();
	if (animTimer_ > 0.3f)
	{
		animFrame_ = (animFrame_ + 1) % 4;
		animTimer_ = animTimer_ - ANIM_INTERVAL;
	}
}
//turnRight(),turnLeft()
//reverse(),forward()を実装するか
//Point GetXYDistance();



void Enemy::YCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	if (pos_.y > player->GetPos().y)
	{
		forward_ = UP;
	}
	else if (pos_.y < player->GetPos().y)
	{
		forward_ = DOWN;
	}
}

void Enemy::XCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	if (pos_.x > player->GetPos().x)
	{
		forward_ = LEFT;
	}
	else if (pos_.x < player->GetPos().x)
	{
		forward_ = RIGHT;
	}
}

void Enemy::XYCloserMove()
{
	Player* player = (Player*)FindGameObject<Player>();
	int xdis = abs(pos_.x - player->GetPos().x);
	int ydis = abs(pos_.y - player->GetPos().y);

	if (xdis > ydis) {
		if (pos_.x > player->GetPos().x)
		{
			forward_ = LEFT;
		}
		else if (pos_.x < player->GetPos().x)
		{
			forward_ = RIGHT;
		}
	}
	else
	{
		if (pos_.y > player->GetPos().y)
		{
			forward_ = UP;
		}
		else if (pos_.y < player->GetPos().y)
		{
			forward_ = DOWN;
		}
	}
}

void Enemy::XYCloserMoveRandom()
{

	//３分の1の確率でプレイヤーに近い方に行く、残りの3分の1はランダム方向に移動、残りは何もしない
	Player* player = (Player*)FindGameObject<Player>();
	int xdis = abs(pos_.x - player->GetPos().x);
	int ydis = abs(pos_.y - player->GetPos().y);
	int rnum = GetRand(2);
	if (rnum == 0)
		XYCloserMove();
	else if (rnum == 1)
	{
		forward_ = (DIR)GetRand(3);
	}
}



void Enemy::Draw()
{
	if (isGraphic)
	{
		DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, frameNum[animFrame_] * 32, yTerm[forward_]*32, 32, 32, enemyImage_, TRUE);
	}
	else {


		DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT,
			GetColor(80, 89, 10), TRUE);
		Point tp[4][3] = {
			{{pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
			{{pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}, {pos_.x, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}},
			{{pos_.x            , pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}},
			{{pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT / 2}, {pos_.x + CHA_WIDTH / 2, pos_.y}, {pos_.x + CHA_WIDTH / 2, pos_.y + CHA_HEIGHT}}
		};

		DrawTriangle(tp[forward_][0].x, tp[forward_][0].y, tp[forward_][1].x, tp[forward_][1].y, tp[forward_][2].x, tp[forward_][2].y, GetColor(255, 255, 255), TRUE);
	}
}

bool Enemy::CheckHit(const Rect& me, const Rect& other)
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

bool Enemy::isHitWall(const Rect& me)
{
	Stage* stage = (Stage*)FindGameObject<Stage>();
	for (int i = 0; i < NEIGHBOURS; i++) {
		int x = me.x / CHA_WIDTH + nineNeibor[i].x;
		int y = me.y / CHA_HEIGHT + nineNeibor[i].y;

		StageObj& tmp = stage->GetStageGrid()[y][x];
		if (tmp.type == STAGE_OBJ::EMPTY) 
			continue;

		if (CheckHit(me, tmp.rect))
		{
			if (tmp.type == STAGE_OBJ::BRICK || tmp.type == WALL)
			{
				return true;
			}
		}
	}
	return false;
}


//
//void Enemy::RightHandMove()
//{
//	DIR myRight[4] = { RIGHT, LEFT, UP, DOWN };
//	DIR myLeft[4] = { LEFT, RIGHT, DOWN, UP };
//	Point nposF = { pos_.x + nDir[forward_].x, pos_.y + nDir[forward_].y };
//	Point nposR = { pos_.x + nDir[myRight[forward_]].x, pos_.y + nDir[myRight[forward_]].y };
//	Rect myRectF{ nposF.x, nposF.y, CHA_WIDTH, CHA_HEIGHT };
//	Rect myRectR{ nposR.x, nposR.y, CHA_WIDTH, CHA_HEIGHT };
//	Stage* stage = (Stage*)FindGameObject<Stage>();
//	bool isRightOpen = true;
//	bool isForwardOpen = true;
//	//for (auto& obj : stage->GetStageRects()) {
//	for (int y = 0; y < STAGE_HEIGHT; y++)
//	{
//		for (int x = 0; x < STAGE_WIDTH; x++)
//		{
//			Rect tmp = stage->GetStageGrid()[y][x].rect;
//			if (CheckHit(myRectF, tmp)) {
//				isForwardOpen = false;
//			}
//			if (CheckHit(myRectR, tmp)) {
//				isRightOpen = false;
//			}
//		}
//		if (isRightOpen)
//		{
//			forward_ = myRight[forward_];
//		}
//		else if (isRightOpen == false && isForwardOpen == false)
//		{
//			forward_ = myLeft[forward_];
//		}
//	}
//}

//void Enemy::Dijkstra(Point sp, Point gp)
//{
//	using Mdat = std::pair<int, Point>;
//
//	dist[sp.y][sp.x] = 0;
//	std::priority_queue<Mdat, std::vector<Mdat>, std::greater<Mdat>> pq;
//	pq.push(Mdat(0, { sp.x, sp.y }));
//	vector<vector<StageObj>> stageData = ((Stage*)FindGameObject<Stage>())->GetStageGrid();
//
//	while (!pq.empty())
//	{
//		Mdat p = pq.top();
//		pq.pop();
//
//		//Rect{ (int)p.second.x * STAGE_WIDTH, (int)p.second.y * BLOCK_SIZE.y, BLOCK_SIZE }.draw(Palette::Red);
//		//getchar();
//		int c = p.first;
//		Point v = p.second;
//		
//		for (int i = 0; i < 4; i++)
//		{
//			Point np = { v.x + (int)nDir[i].x, v.y + (int)nDir[i].y };
//			if (np.x < 0 || np.y < 0 || np.x >= STAGE_WIDTH || np.y >= STAGE_HEIGHT) continue;
//			if (stageData[np.y][np.x].obj == STAGE_OBJ::WALL) continue;
//			if (dist[np.y][np.x] <= stageData[np.y][np.x].weight + c) continue;
//			dist[np.y][np.x] = stageData[np.y][np.x].weight + c;
//			pre[np.y][np.x] = Point({ v.x, v.y });
//			pq.push(Mdat(dist[np.y][np.x], np));
//		}
//	}
//}