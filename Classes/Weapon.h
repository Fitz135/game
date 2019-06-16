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
	void(Weapon::*BulletEnd[3])(Node* who) = { &Weapon::ArrowEnd,&Weapon::StarEnd,&Weapon::BubbleEnd};
	float WeaponSpeed[5] = { 1.0f / 2,1.0f / 2,1.0f / 8,1.0f,1.0f / 2 };
	void Bow(Point MousePosition);
	void ArrowEnd(Node * who);
	void StarEnd(Node* who);
	void BubbleEnd(Node* who);
	void Sword(Point MousePosition);
	void BubbleGun(Point MousePosition);
	void Lance(Point MousePosition);
	void WeaponBack(Node* who);
	void Boomerang(Point MousePosition);
	void BoomerangBack(float dt);
	void disappear(Node * who);
};
