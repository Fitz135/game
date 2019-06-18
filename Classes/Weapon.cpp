#include"Weapon.h"
#include"GameScene.h"
#include"Settings.h"
#include"Player.h"
#include "SimpleAudioEngine.h"

#define PI 3.1415

Weapon* Weapon::create(int WeaponType)
{
	auto weapon = Weapon::create();
	weapon->MyWeapon = Sprite::create(settings::weapon_paths[WeaponType]);
	weapon->MyWeapon->retain();
	weapon->MyWeapon->setOpacity(0);
	weapon->WeaponMode = weapon->weaponmode[WeaponType];
	return weapon;
}
void Weapon::Bow(Point TouchPosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/bow.wav", false);

	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto scene = GameScene::getCurrentScene();

	auto playerPosition = player->getPosition();
	player->AttackMode1(TouchPosition);

	Point BowPosition;
	float dir1 = TouchPosition.x - playerPosition.x - TouchPosition.y + playerPosition.y;
	float dir2 = TouchPosition.x - playerPosition.x + TouchPosition.y - playerPosition.y;
	if (dir1 < 0 && dir2>0)
		BowPosition = Vec2(5, 7);
	else if (dir1*dir2 >= 0)
		BowPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BowPosition = Vec2(5, -11);

	int K, J;
	if (TouchPosition.x > playerPosition.x)	K = 1;
	else K = -1;
	if ((TouchPosition.x - (playerPosition.x + K * BowPosition.x)) < 0)J = 1;
	else J = -1;

	auto Theta = atanf((TouchPosition.y - playerPosition.y - BowPosition.y) / (TouchPosition.x - (playerPosition.x + K * BowPosition.x)));
	
	MyWeapon->setRotation((-PI / 2 - J * K*PI / 2 - K * Theta) / PI * 180);;
	MyWeapon->setAnchorPoint(Vec2(0.28125, 0.5));
	MyWeapon->setPosition(BowPosition);
	MyWeapon->setOpacity(255);

	auto arrow = Sprite::create("arrow.png");
	auto arrowPosition = Vec2(playerPosition.x + K * BowPosition.x, playerPosition.y + BowPosition.y);

	double range = 200;
	float x = range * cos(PI / 2 + J * PI / 2 + Theta) + arrowPosition.x;
	float y = range * sin(PI / 2 + J * PI / 2 + Theta) + arrowPosition.y;
	auto move = MoveTo::create(1.0f/2, Vec2(x, y));
	auto arrowend = CallFuncN::create(this, callfuncN_selector(Weapon::ArrowEnd));
	auto attackmove = Sequence::create(move, arrowend, NULL);
	attackmove->setTag(100);

	arrow->setPosition(arrowPosition);
	arrow->setRotation((-Theta + J * PI / 2 + PI) / PI * 180);
	arrow->setAnchorPoint(Vec2(0.5, 0.31250));
	arrow->runAction(attackmove);

	arrow->setTag(player->getTag());
	arrow->setName("Arrow");
	scene->Bulletset->addChild(arrow,1);
	scene->Bullets->addObject(arrow);
}
void Weapon::ArrowEnd(Node* who)
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	for (int i = 0; i <= 4; i++)
		animFrames.pushBack(spritecache->getSpriteFrameByName("explode1/explode1-" + std::to_string(i) + ".png"));
	auto boom = Animate::create(Animation::createWithSpriteFrames(animFrames, 1.0f / 10));
	auto disappear = CallFuncN::create(this, callfuncN_selector(Weapon::disappear));
	auto seq = Sequence::create(boom, disappear, nullptr);
	who->runAction(seq);
}
void Weapon::Sword(Point TouchPosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/sword.wav", false);

	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto scene = dynamic_cast<GameScene*>(player->getParent()->getParent());

	auto playerPosition = player->getPosition();
	player->AttackMode2(TouchPosition);

	auto rotate = RotateBy::create(6.0*WeaponSpeed[1] / 10, 180);
	auto delay = DelayTime::create(1.0*WeaponSpeed[1] / 10);
	auto move = MoveBy::create(1.0*WeaponSpeed[1] / 10, Vec2(14, 0));
	auto move2 = MoveBy::create(1.0*WeaponSpeed[1] / 10, Vec2(2, -14));
	auto move3 = MoveBy::create(1.0*WeaponSpeed[1] / 10, Vec2(-2, -4));
	auto seq = Sequence::create(delay, move, delay, move2, delay, move3, nullptr);

	MyWeapon->setOpacity(255);
	MyWeapon->setAnchorPoint(Vec2(0.15909, 0.11429));
	MyWeapon->setPosition(-9, 7);
	MyWeapon->setRotation(-90);//once
	MyWeapon->runAction(rotate);
	MyWeapon->runAction(seq);

	auto visiblesize = Director::getInstance()->getOpenGLView()->getVisibleSize();
	Point StarPosition;
	if (TouchPosition.x > playerPosition.x)
		StarPosition.x = 1280 + 90;
	else StarPosition.x = -90;
	if (TouchPosition.y > playerPosition.y)
		StarPosition.y = 1280 + 90;
	else
		StarPosition.y = -90;

	auto starlight = Sprite::create("Starlight.png");
	auto star = Sprite::create("Star2.png");

	star->setPosition(17, 18);
	star->setRotation(180);
	starlight->setAnchorPoint(Vec2(0.5, 0.2));
	starlight->addChild(star);
	starlight->setPosition(StarPosition);

	auto l = sqrt((TouchPosition.y - StarPosition.y)*(TouchPosition.y - StarPosition.y) + (TouchPosition.x - StarPosition.x)*(TouchPosition.x - StarPosition.x));
	auto Theta = atanf((TouchPosition.y - StarPosition.y) / (TouchPosition.x - StarPosition.x));
	if (TouchPosition.x - StarPosition.x > 0)Theta += PI;
	starlight->setRotation(-Theta / PI * 180 + 90);

	auto move4 = MoveTo::create(1.0f / 600 * l, TouchPosition);
	auto starend = CallFuncN::create(this, callfuncN_selector(Weapon::StarEnd));
	auto attackmove = Sequence::create(move4, starend, nullptr);

	attackmove->setTag(100);
	starlight->setTag(player->getTag());
	starlight->setName("Starlight");
	scene->Bulletset->addChild(starlight,100);
	scene->Bullets->addObject(starlight);

	auto light = ParticleExplosion::createWithTotalParticles(3);
	light->setEmitterMode((cocos2d::ParticleSystem::Mode)1);
	light->setTexture(Director::getInstance()->getTextureCache()->addImage("Star.png"));
	light->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	light->setStartColorVar(Color4F(0, 0, 0, 0));
	light->setEndColorVar(Color4F(0, 0, 0, 0));
	light->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	light->setStartSize(10.0f);
	light->setStartSizeVar(2.0f);
	light->setEndSizeVar(0);
	light->setLife(1.0f / 2);
	light->setLifeVar(1.0f / 4);
	light->setDuration(1.0f / 300 * l);
	light->setAngle(-90);
	light->setStartRadiusVar(0);
	light->setStartRadius(10);
	light->setEndRadius(65);
	light->setEndRadiusVar(25);
	light->setAngleVar(20);
	light->setPosition(17, 0);
	light->setPositionType(kCCPositionTypeGrouped);
	starlight->addChild(light);
	starlight->runAction(attackmove);
}
void Weapon::StarEnd(Node* who)
{
	auto boom = ParticleExplosion::createWithTotalParticles(10);
	auto Radius = who->getContentSize().width / who->getScale();
	boom->setEmitterMode((cocos2d::ParticleSystem::Mode)1);
	boom->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	boom->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	boom->setStartColorVar(Color4F(0, 0, 0, 0));
	boom->setEndColorVar(Color4F(0, 0, 0, 0));
	boom->setStartSize(15.0f);
	boom->setStartSizeVar(2.0f);
	boom->setEndSize(0);
	boom->setEndSizeVar(0);
	boom->setStartRadius(0);
	boom->setStartRadiusVar(0);
	boom->setEndRadius(50);
	boom->setEndRadiusVar(10);
	boom->setTexture(Director::getInstance()->getTextureCache()->addImage("Star.png"));
	boom->setLife(1.0f / 2);
	boom->setLifeVar(0);
	boom->setPosition(who->getPosition());

	auto map = who->getParent();
	if (map)map->addChild(boom,2);
	disappear(who);
}
void Weapon::BubbleGun(Point TouchPosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/Bubblegun.wav", false);

	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto scene = dynamic_cast<GameScene*>(player->getParent()->getParent());

	auto playerPosition = player->getPosition();
	player->AttackMode1(TouchPosition);

	Point BubbleGunPosition;
	float dir1 = TouchPosition.x - playerPosition.x - TouchPosition.y + playerPosition.y;
	float dir2 = TouchPosition.x - playerPosition.x + TouchPosition.y - playerPosition.y;
	if (dir1 < 0 && dir2>0)
		BubbleGunPosition = Vec2(5, 7);
	else if (dir1*dir2 >= 0)
		BubbleGunPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BubbleGunPosition = Vec2(5, -11);
	int K, J;
	if (TouchPosition.x > playerPosition.x)K = 1;
	else K = -1;
	if ((TouchPosition.x - (playerPosition.x + K * BubbleGunPosition.x)) < 0)J = 1;
	else J = -1;
	auto Theta = atanf((TouchPosition.y - playerPosition.y - BubbleGunPosition.y) / (TouchPosition.x - (playerPosition.x + K * BubbleGunPosition.x)));
	
	MyWeapon->setRotation((-PI / 2 - J * K*PI / 2 - K * Theta) / PI * 180);;
	MyWeapon->setOpacity(255);
	MyWeapon->setPosition(BubbleGunPosition);
	MyWeapon->setAnchorPoint(Vec2(0.50000, 0.30000));//once

	auto Bubble = Sprite::create("Bubble.png");
	auto BubblePosition = Vec2(playerPosition.x + K * BubbleGunPosition.x, playerPosition.y + BubbleGunPosition.y) + Vec2(35.69* cos(PI / 2 + J * PI / 2 + Theta + K * 0.197), 35.69* sin(PI / 2 + J * PI / 2 + Theta + K * 0.197));

	srand(int(time(0)) + rand());
	float scale = (float)(rand() % 6) / 10 + 1.0;
	float range = (float)(rand() % 100) + 100;
	float rdTheta = (float)(rand() % 314) / 300 - PI / 6;
	float x = range * cos(PI / 2 + J * PI / 2 + Theta + rdTheta) + BubblePosition.x;
	float y = range * sin(PI / 2 + J * PI / 2 + Theta + rdTheta) + BubblePosition.y;
	auto move = MoveTo::create(1.5f, Vec2(x, y));
	auto bubbleend = CallFuncN::create(this, callfuncN_selector(Weapon::BubbleEnd));
	auto attackmove = Sequence::create(move, bubbleend, NULL);

	Bubble->setOpacity(200);
	Bubble->setPosition(BubblePosition);
	Bubble->setScale(scale);
	Bubble->runAction(attackmove);
	Bubble->setName("Bubble");
	attackmove->setTag(100);
	Bubble->setTag(player->getTag());
	scene->Bulletset->addChild(Bubble,1);
	scene->Bullets->addObject(Bubble);
}
void Weapon::BubbleEnd(Node* who)
{
	auto boom = ParticleExplosion::createWithTotalParticles(15);
	auto Radius = who->getContentSize().width / who->getScale();
	boom->setEmitterMode((cocos2d::ParticleSystem::Mode)1);
	boom->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	boom->setStartColorVar(Color4F(0, 0, 0, 0));
	boom->setEndColorVar(Color4F(0, 0, 0, 0));
	boom->setStartSize(2.5f);
	boom->setStartSizeVar(0);
	boom->setStartRadius(Radius);
	boom->setEndRadius(Radius*1.75);
	boom->setEndRadiusVar(Radius* 0.5);
	boom->setTexture(Director::getInstance()->getTextureCache()->addImage("Bubble.png"));
	boom->setLife(1.0f / 4);
	boom->setLifeVar(0);
	boom->setPosition(who->getPosition());

	auto map = who->getParent();
    if(map)map->addChild(boom,2);
	disappear(who);
}
void Weapon::Boomerang(Point TouchPosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/Bubblegun.wav", false);

	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto scene =dynamic_cast <GameScene*>(player->getParent()->getParent());
	
	auto playerPosition = player->getPosition();
	player->AttackMode1(TouchPosition);
	player->IsHaveWeapon = 0;
	boomerang = Sprite::create(AutoPolygon::generatePolygon("Boomerang.png"));

	int K;
	if (TouchPosition.x > playerPosition.x)K = 1;
	else K = -1;

	Point BoomerangPosition;
	float dir1 = TouchPosition.x - playerPosition.x - TouchPosition.y + playerPosition.y;
	float dir2 = TouchPosition.x - playerPosition.x + TouchPosition.y - playerPosition.y;
	if (dir1 < 0 && dir2>0)
		BoomerangPosition = Vec2(playerPosition.x + K * 5, playerPosition.y + 7);
	else if (dir1*dir2 >= 0)
		BoomerangPosition = Vec2(playerPosition.x + K * 7, playerPosition.y - 7);
	else if (dir1 > 0 && dir2 < 0)
		BoomerangPosition = Vec2(playerPosition.x + K * 5, playerPosition.y - 11);

	auto rotate = RotateBy::create(30.0f / 60, 360);
	auto move = RepeatForever::create(rotate);
	double range = 200;
	double l = sqrt((TouchPosition.x - playerPosition.x)*(TouchPosition.x - playerPosition.x) + (TouchPosition.y - playerPosition.y)*(TouchPosition.y - playerPosition.y));
	double x = range * (TouchPosition.x - playerPosition.x) / l + playerPosition.x;
	double y = range * (TouchPosition.y - playerPosition.y) / l + playerPosition.y;
	auto move2 = MoveTo::create(25.0f / 60, Vec2(x, y));

	boomerang->setPosition(BoomerangPosition);
	boomerang->runAction(move);
	boomerang->runAction(move2);
	boomerang->setName("Boomerang");
	boomerang->setTag(player->getTag());
	scene->Bulletset->addChild(boomerang,100);
	scene->Bullets->addObject(boomerang);

	player->schedule(schedule_selector(Weapon::BoomerangBack), 2.0f / 60);
}
void Weapon::BoomerangBack(float dt)
{
	auto player =(Player*)(this);
	if (player->weapon->boomerang&&player->weapon->boomerang->numberOfRunningActions() == 1)
	{
		auto the_boomerang = player->weapon->boomerang;
		auto playerPosition = player->getPosition();
		auto boomerangPosition = the_boomerang->getPosition();
		double l = sqrt((playerPosition.x - boomerangPosition.x)*(playerPosition.x - boomerangPosition.x) + (playerPosition.y - boomerangPosition.y)*(playerPosition.y - boomerangPosition.y));
		if (l <= 30)
		{
			player->IsHaveWeapon = 1;
			auto scene =dynamic_cast<GameScene*>(player->getParent()->getParent());
			scene->Bullets->removeObject(the_boomerang);
			disappear(the_boomerang);
			player->unschedule(schedule_selector(Weapon::BoomerangBack));
		}
		else
		{
			double range = 20;
			double x = range * (playerPosition.x - boomerangPosition.x) / l + boomerangPosition.x;
			double y = range * (playerPosition.y - boomerangPosition.y) / l + boomerangPosition.y;
			auto move = MoveTo::create(2.0f / 60, Vec2(x, y));
			the_boomerang->runAction(move);
		}
	}
}
void Weapon::Lance(Point TouchPosition)
{
	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto scene = dynamic_cast <GameScene*>(player->getParent()->getParent());
	auto playerPosition = player->getPosition();

	if (sqrt((playerPosition.x - TouchPosition.x)*(playerPosition.x - TouchPosition.x) + (playerPosition.y - TouchPosition.y)*(playerPosition.y - TouchPosition.y)) <= 84)
	{
		player->AttackMode2(TouchPosition);

		auto rotate = RotateBy::create(6.0*WeaponSpeed[4] / 10, 360);
		auto delay = DelayTime::create(1.0*WeaponSpeed[4] / 10);
		auto move = MoveBy::create(1.0*WeaponSpeed[4] / 10, Vec2(14, 0));
		auto move2 = MoveBy::create(1.0*WeaponSpeed[4] / 10, Vec2(2, -14));
		auto move3 = MoveBy::create(1.0*WeaponSpeed[4] / 10, Vec2(-2, -4));
		auto seq = Sequence::create(delay, move, delay, move2, delay, move3, nullptr);
		seq->setTag(10);
		rotate->setTag(10);

		MyWeapon->setOpacity(255);
		MyWeapon->setPosition(-9, 7);
		MyWeapon->setRotation(-90);
		MyWeapon->runAction(rotate);
		MyWeapon->runAction(seq);
	}
	else
	{
		player->AttackMode1(TouchPosition);

		MyWeapon->stopAllActionsByTag(10);
		Point BowPosition;
		float dir1 = TouchPosition.x - playerPosition.x - TouchPosition.y + playerPosition.y;
		float dir2 = TouchPosition.x - playerPosition.x + TouchPosition.y - playerPosition.y;
		if (dir1 < 0 && dir2>0)
			BowPosition = Vec2(5, 7);
		else if (dir1*dir2 >= 0)
			BowPosition = Vec2(7, -7);
		else if (dir1 > 0 && dir2 < 0)
			BowPosition = Vec2(5, -11);

		int K, J;
		if (TouchPosition.x > playerPosition.x)	K = 1;
		else K = -1;
		if ((TouchPosition.x - (playerPosition.x + K * BowPosition.x)) < 0)J = 1;
		else J = -1;

		auto Theta = atanf((TouchPosition.y - playerPosition.y - BowPosition.y) / (TouchPosition.x - (playerPosition.x + K * BowPosition.x)));
		MyWeapon->setRotation((-PI / 4 - J * K*PI / 2 - K * Theta) / PI * 180);;
		MyWeapon->setPosition(BowPosition);
		MyWeapon->setOpacity(255);

		double range = 84;
		float x = range * cos(PI / 2 + J * PI / 2 + Theta);
		float y = range * sin(PI / 2 + J * PI / 2 + Theta);
		auto move = MoveBy::create(3.0*WeaponSpeed[4] / 10, Vec2(K*x, y));
		auto back = CallFuncN::create(this, callfuncN_selector(Weapon::WeaponBack));
		auto seq = Sequence::create(move, move->reverse(), back, nullptr);
		MyWeapon->runAction(seq);
	}
}
void Weapon::Shield(Point TouchPosition)
{
	MyWeapon->setOpacity(255);
	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	auto playerPosition = player->getPosition();
	player->MoveSpeed = 24;
	player->AttackMode3(TouchPosition);
}
void Weapon::disappear(Node* who)
{
	who->removeFromParentAndCleanup(TRUE);
}
void Weapon::WeaponBack(Node* who)
{
	auto player = dynamic_cast<Player*>(MyWeapon->getParent());
	player->IsHaveWeapon = 1;
}