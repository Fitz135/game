#pragma once
#include"cocos2d.h"

using namespace cocos2d;

class Weapon :public cocos2d::Layer
{
public:
	CREATE_FUNC(Weapon);
	Sprite *MyWeapon;
	Sprite* boomerang;
	static Weapon * create(int WeaponType);
	void(Weapon::*WeaponMode)(Point MousePosition);
	
	void(Weapon::*BulletEnd[3])(Node* who) = { 
		&Weapon::ArrowEnd,
		&Weapon::StarEnd,
		&Weapon::BubbleEnd
	};
	const float WeaponSpeed[6] = { 
		1.0f/2,
		1.0f / 2,
		1.0f / 8,
		1.0f,
		1.0f / 2,
		1.0f 
	};
private:
	void(Weapon::*weaponmode[6])(Point MousePosition) = {
		&Weapon::Bow,
		&Weapon::Sword,
		&Weapon::BubbleGun,
		&Weapon::Boomerang,
		&Weapon::Lance,
		&Weapon::Shield
	};

	void Bow(Point MousePosition);
	void ArrowEnd(Node * who);

	void Sword(Point MousePosition);
	void StarEnd(Node* who);

	void BubbleEnd(Node* who);
	void BubbleGun(Point MousePosition);

	void Boomerang(Point MousePosition);
	void BoomerangBack(float dt);

	void Lance(Point MousePosition);

	void Shield(Point MousePosition);

	void WeaponBack(Node* who);
	void disappear(Node * who);
};
