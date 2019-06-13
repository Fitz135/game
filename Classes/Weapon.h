#pragma once
#include"cocos2d.h"

using namespace cocos2d;

class Weapon :public cocos2d::Sprite
{
public:
	CREATE_FUNC(Weapon);
	Sprite *MyWeapon;
	static Weapon * create(int WeaponType);
	void(Weapon::*WeaponMode)(Point MousePosition);
	float WeaponSpeed[4] = {
		1.0f,
		1.0f,
		1.0f / 5,
		1.0f 
	};
	void Bow(Point MousePosition);
	void Sword(Point MousePosition);
	void BubbleGun(Point MousePosition);
	void Boomerang(Point MousePosition);
	void BoomerangBack(float dt);
	void disappear(Node * who);
};
