#include"Weapon.h"
#include"GameScene.h"
#include"Settings.h"
#include"Player.h"

#define PI 3.1415

void(Weapon::*weaponmode[4])(Point MousePosition) = { &Weapon::Bow,&Weapon::Sword,&Weapon::BubbleGun,&Weapon::Boomerang };

inline Player* getMyplayer(char* str) {
	auto scene = GameScene::getCurrentMap();
	return dynamic_cast<Player*>(scene->getChildByName(str));
}
Weapon* Weapon::create(int WeaponType)
{
	auto weapon = Weapon::create();
	weapon->MyWeapon = Sprite::create(settings::weapon_paths[WeaponType]);
	weapon->MyWeapon->retain();
	weapon->MyWeapon->setOpacity(0);
	weapon->MyWeapon->setZOrder(1);
	weapon->WeaponMode = weaponmode[WeaponType];

	return weapon;
}
void Weapon::Bow(Point MousePosition)
{
	auto scene = GameScene::getCurrentMap();
	auto Player = getMyplayer("Player");
	auto PlayerPosition = Player->getPosition();

	Point BowPosition;
	float dir1 = MousePosition.x - PlayerPosition.x - MousePosition.y + PlayerPosition.y;
	float dir2 = MousePosition.x - PlayerPosition.x + MousePosition.y - PlayerPosition.y;
	if (dir1 < 0 && dir2>0)
		BowPosition = Vec2(5, 7);
	else if (dir1*dir2 >= 0)
		BowPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BowPosition = Vec2(5, -11);

	int K,J;
	if (MousePosition.x > PlayerPosition.x)	K = 1;
	else K = -1;
	if ((MousePosition.x - (PlayerPosition.x + K * BowPosition.x)) < 0)J = 1;
	else J = -1;

	auto Theta = atanf((MousePosition.y - PlayerPosition.y - BowPosition.y) / (MousePosition.x - (PlayerPosition.x + K * BowPosition.x)));
	MyWeapon->setRotation((-PI / 2 - J * K*PI / 2 - K * Theta) / PI * 180);;
	MyWeapon->setAnchorPoint(Vec2(0.28125, 0.5));//once
	MyWeapon->setPosition(BowPosition);
	MyWeapon->setOpacity(255);

	auto Arrow = Sprite::create("arrow.png");
	auto ArrowPosition=Vec2(PlayerPosition.x + K * BowPosition.x, PlayerPosition.y + BowPosition.y);
	
	double range = 200;
	float x = range * cos(PI / 2 + J * PI / 2 + Theta) + ArrowPosition.x ;
	float y = range * sin(PI / 2 + J * PI / 2 + Theta) + ArrowPosition.y ;
	auto move = MoveTo::create(1, Vec2(x, y));
	auto disappear = CallFuncN::create(this, callfuncN_selector(Weapon::disappear));
	auto attackmove = Sequence::create(move,/* FadeOut::create(30.0f/60), */disappear, NULL);

	Arrow->setPosition(ArrowPosition);
	Arrow->setRotation((-Theta + J * PI / 2) / PI * 180);
	Arrow->setAnchorPoint(Vec2(0.5, 0.84375));
	Arrow->runAction(attackmove);
	scene->addChild(Arrow);
}
void Weapon::Sword(Point MousePosition)
{
	auto Player = getMyplayer("Player");
	auto PlayerPosition = Player->getPosition();

	auto rotate = RotateBy::create(6.0f / 16, 180);
	auto delay = DelayTime::create(1.0f / 16);
	auto move = MoveBy::create(1.0f / 16, Vec2(14, 0));
	auto move2 = MoveBy::create(1.0f / 16, Vec2(2, -14));
	auto move3 = MoveBy::create(1.0f / 16, Vec2(-2, -4));
	auto seq = Sequence::create(delay, move, delay, move2, delay, move3, nullptr);

	MyWeapon->setOpacity(255);
	MyWeapon->setAnchorPoint(Vec2(0.09259, 0.09259));//once
	MyWeapon->setPosition(-9, 7);
	MyWeapon->setRotation(-90);//once
	MyWeapon->runAction(rotate);
	MyWeapon->runAction(seq);
}

void Weapon::BubbleGun(Point MousePosition)
{
	auto scene = GameScene::getCurrentMap();
	auto Player = getMyplayer("Player");
	auto PlayerPosition = Player->getPosition();
	
	Point BubbleGunPosition;
	float dir1 = MousePosition.x - PlayerPosition.x - MousePosition.y + PlayerPosition.y;
	float dir2 = MousePosition.x - PlayerPosition.x + MousePosition.y - PlayerPosition.y;
	if (dir1 < 0 && dir2>0)
		BubbleGunPosition = Vec2( 5, 7);
	else if (dir1*dir2 >= 0)
		BubbleGunPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BubbleGunPosition = Vec2( 5,- 11);
	
	int K,J;
	if (MousePosition.x > PlayerPosition.x)K = 1;
	else K = -1;
	if ((MousePosition.x - (PlayerPosition.x + K * BubbleGunPosition.x)) < 0)J = 1;
	else J = -1;

	auto Theta =atanf((MousePosition.y - PlayerPosition.y-BubbleGunPosition.y) / (MousePosition.x - (PlayerPosition.x+K*BubbleGunPosition.x)));
	MyWeapon->setRotation((-PI/2-J*K*PI/2-K*Theta)/ PI * 180);;
	MyWeapon->setOpacity(255);
	MyWeapon->setPosition(BubbleGunPosition);
	MyWeapon->setAnchorPoint(Vec2(0.50000, 0.30000));//once

	auto Bubble = Sprite::create("Bubble.png");
	auto BubblePosition = Vec2(PlayerPosition.x + K * BubbleGunPosition.x, PlayerPosition.y + BubbleGunPosition.y) + Vec2(35.69* cos(PI/2+J*PI/2 +Theta +K*0.197), 35.69* sin(PI / 2 + J * PI / 2 +Theta + K*0.197));
	
	srand(int(time(0)) + rand());
	float scale = (float)(rand() % 6) / 10 + 1.0;
	float range = (float)(rand() % 100) + 100;
	float rdTheta = (float)(rand() % 314) / 300 - PI / 6;
	float x = range * cos(PI / 2 + J * PI / 2 +Theta + rdTheta) + BubblePosition.x;
    float y = range * sin(PI / 2 + J * PI / 2 +Theta + rdTheta) + BubblePosition.y;
	auto move = MoveTo::create(1.5f, Vec2(x,y));
	auto disappear = CallFuncN::create(this, callfuncN_selector(Weapon::disappear));
	auto attackmove = Sequence::create(move, disappear, NULL);
	
	Bubble->setOpacity(200);
	Bubble->setPosition(BubblePosition);
	Bubble->setScale(scale);
	Bubble->runAction(attackmove);
	scene->addChild(Bubble);
}

Sprite* boomerang;
void Weapon::Boomerang(Point MousePosition)
{
	    auto scene = GameScene::getCurrentMap();
	    auto Player = getMyplayer("Player");
		auto PlayerPosition = Player->getPosition();
		Player->IsHaveWeapon = 0;
		boomerang = Sprite::create(AutoPolygon::generatePolygon("Boomerang.png"));

		int K;
		if (MousePosition.x > PlayerPosition.x)K = 1;
		else K = -1;

		Point BoomerangPosition;
		float dir1 = MousePosition.x - PlayerPosition.x - MousePosition.y + PlayerPosition.y;
		float dir2 = MousePosition.x - PlayerPosition.x + MousePosition.y - PlayerPosition.y;
		if (dir1 < 0 && dir2>0)
			BoomerangPosition = Vec2(PlayerPosition.x + K * 5, PlayerPosition.y + 7);
		else if (dir1*dir2 >= 0)
			BoomerangPosition = Vec2(PlayerPosition.x + K * 7, PlayerPosition.y - 7);
		else if (dir1 > 0 && dir2 < 0)
			BoomerangPosition = Vec2(PlayerPosition.x + K * 5, PlayerPosition.y - 11);

		auto rotate = RotateBy::create(30.0f / 60, 360);
		auto move = RepeatForever::create(rotate);
		double range = 200;
		double l = sqrt((MousePosition.x - PlayerPosition.x)*(MousePosition.x - PlayerPosition.x) + (MousePosition.y - PlayerPosition.y)*(MousePosition.y - PlayerPosition.y));
		double x = range * (MousePosition.x - PlayerPosition.x) / l + PlayerPosition.x;
		double y = range * (MousePosition.y - PlayerPosition.y) / l + PlayerPosition.y;
		auto move2 = MoveTo::create(25.0f / 60, Vec2(x, y));

		boomerang->setPosition(BoomerangPosition);
		boomerang->runAction(move);
		boomerang->runAction(move2);
		scene->addChild(boomerang);
		Player->schedule(schedule_selector(Weapon::BoomerangBack), 2.0f / 60);
}
void Weapon::BoomerangBack(float dt)
{
	if (boomerang->numberOfRunningActions() == 1)
	{
		auto Player = getMyplayer("Player");
		auto PlayerPosition = Player->getPosition();
		auto BoomerangPosition = boomerang->getPosition();
		double l = sqrt((PlayerPosition.x - BoomerangPosition.x)*(PlayerPosition.x - BoomerangPosition.x) + (PlayerPosition.y - BoomerangPosition.y)*(PlayerPosition.y - BoomerangPosition.y));
		if (l<=30)
		{
			Player->IsHaveWeapon = 1;
			disappear(boomerang);
			Player->unschedule(schedule_selector(Weapon::BoomerangBack));
		}
		else 
		{
			double range = 20;
			double x = range * (PlayerPosition.x - BoomerangPosition.x) / l + BoomerangPosition.x;
			double y = range * (PlayerPosition.y - BoomerangPosition.y) / l + BoomerangPosition.y;
			auto move = MoveTo::create(2.0f / 60, Vec2(x, y));
			boomerang->runAction(move);
		}
	}
}
void Weapon::disappear(Node* who)
{
	who->removeFromParentAndCleanup(TRUE);
}