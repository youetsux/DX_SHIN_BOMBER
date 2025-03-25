#include "Item.h"

Item::Item(Point pos, ITEMS type)
	:GameObject(), isInBrick_(true), type_(type),isUsed_(false),hImage_(-1),pos_(pos)
{
	LoadImage();
}

Item::Item()
	:GameObject(), isInBrick_(true), type_(ITEMS::ITEM_FIRE),isUsed_(false),hImage_(-1)
{
	LoadImage();
}

Item::~Item()
{
}

void Item::LoadImage()
{
	switch (type_)
	{
		case ITEMS::ITEM_FIRE:
			hImage_ = LoadGraph("Assets/fup.png");
			break;
		case ITEMS::ITEM_BOMB:
			hImage_ = LoadGraph("Assets/bup.png");
			break;
		case ITEMS::ITEM_SPEED:
			hImage_ = LoadGraph("Assets/spup.png");
			break;
	}
}

void Item::Update()
{


	if (isUsed_)
	{
		this->DestroyMe();
	}
}

void Item::Draw()
{
	if (!isInBrick_)
	{
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, hImage_, TRUE);
	}
}

void Item::SetPos(Point pos)
{
		pos_ = pos;
}

Point Item::GetPos()
{
	return pos_;
}

ITEMS Item::UseItem()
{
	isUsed_ = true;
	return type_;
}
