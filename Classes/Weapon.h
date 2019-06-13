#pragma once
#include"cocos2d.h"

using namespace cocos2d;

class Weapon :public cocos2d::Sprite
{
public:
	CREATE_FUNC(Weapon);
	void RemoveWeapon();
	void Bow(Point MousePosition);
	void Sword(Point MousePosition);
	void BubbleGun(Point MousePosition);
	void Boomerang(Point MousePosition);
	void BoomerangBack(float dt);
	void disappear(Node * who);
};
