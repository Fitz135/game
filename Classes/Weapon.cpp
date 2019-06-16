#include"Weapon.h"
#include"GameScene.h"
#include"Settings.h"
#include"Player.h"
#include "SimpleAudioEngine.h"

#define PI 3.1415

void(Weapon::*weaponmode[5])(Point MousePosition) = { &Weapon::Bow,&Weapon::Sword,&Weapon::BubbleGun,&Weapon::Boomerang,&Weapon::Lance };

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
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/bow.wav", false);

	auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();
	auto scene = (GameScene*)map->getParent();
	auto PlayerPosition = player->getPosition();
	player->AttackMode1(MousePosition);

	Point BowPosition;
	float dir1 = MousePosition.x - PlayerPosition.x - MousePosition.y + PlayerPosition.y;
	float dir2 = MousePosition.x - PlayerPosition.x + MousePosition.y - PlayerPosition.y;
	if (dir1 < 0 && dir2>0)
		BowPosition = Vec2(5, 7);
	else if (dir1*dir2 >= 0)
		BowPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BowPosition = Vec2(5, -11);

	int K, J;
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
	auto ArrowPosition = Vec2(PlayerPosition.x + K * BowPosition.x, PlayerPosition.y + BowPosition.y);

	double range = 200;
	float x = range * cos(PI / 2 + J * PI / 2 + Theta) + ArrowPosition.x;
	float y = range * sin(PI / 2 + J * PI / 2 + Theta) + ArrowPosition.y;
	auto move = MoveTo::create(1, Vec2(x, y));
	auto arrowend = CallFuncN::create(this, callfuncN_selector(Weapon::ArrowEnd));
	auto attackmove = Sequence::create(move, arrowend, NULL);
	attackmove->setTag(0);

	Arrow->setPosition(ArrowPosition);
	Arrow->setRotation((-Theta + J * PI / 2 + PI) / PI * 180);
	Arrow->setAnchorPoint(Vec2(0.5, 0.31250));
	Arrow->runAction(attackmove);
	map->addChild(Arrow);

	Arrow->setTag(0);
	Arrow->setName(player->getName());
	scene->Bullets->addObject(Arrow);
	Arrow->setZOrder(1);
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
void Weapon::Sword(Point MousePosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/sword.wav", false);

	auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();
	auto scene = (GameScene*)map->getParent();
	auto playerPosition = player->getPosition();
	player->AttackMode2(MousePosition);

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
	float dir1 = MousePosition.x - playerPosition.x - MousePosition.y + playerPosition.y;
	float dir2 = MousePosition.x - playerPosition.x + MousePosition.y - playerPosition.y;
	if (dir1 < 0 && dir2>0)
		StarPosition = Vec2(visiblesize.width / 2, visiblesize.height + 90);
	else if (dir1*dir2 >= 0)
	{
		if (MousePosition.x > playerPosition.x)
			StarPosition = Vec2(visiblesize.width + 90, visiblesize.height / 2);
		else
			StarPosition = Vec2(-90, visiblesize.height / 2);
	}
	else if (dir1 > 0 && dir2 < 0)
		StarPosition = Vec2(visiblesize.width / 2, -90);
	auto starlight = Sprite::create("Starlight.png");
	auto star = Sprite::create("Star2.png");
	star->setPosition(17, 18);
	star->setRotation(180);
	starlight->setAnchorPoint(Vec2(0.5, 0.2));
	starlight->addChild(star);
	starlight->setPosition(StarPosition);
	auto l = sqrt((MousePosition.y - StarPosition.y)*(MousePosition.y - StarPosition.y) + (MousePosition.x - StarPosition.x)*(MousePosition.x - StarPosition.x));
	auto Theta = atanf((MousePosition.y - StarPosition.y) / (MousePosition.x - StarPosition.x));
	if (MousePosition.x - StarPosition.x > 0)Theta += PI;

	starlight->setRotation(-Theta / PI * 180 + 90);
	auto move4 = MoveTo::create(1.0f / 300 * l, MousePosition);
	auto starend = CallFuncN::create(this, callfuncN_selector(Weapon::StarEnd));
	auto attackmove = Sequence::create(move4, starend, nullptr);

	map->addChild(starlight);
	attackmove->setTag(1);
	starlight->setTag(1);
	starlight->setName(player->getName());
	scene->Bullets->addObject(starlight);
	starlight->setZOrder(1);

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
	boom->setZOrder(2);
	/*auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();*/
	auto map = who->getParent();
	if (map)
		map->addChild(boom);
	disappear(who);
}
void Weapon::BubbleGun(Point MousePosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/Bubblegun.wav", false);

	auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();
	auto scene = (GameScene*)map->getParent();
	auto PlayerPosition = player->getPosition();
	player->AttackMode1(MousePosition);
	Point BubbleGunPosition;
	float dir1 = MousePosition.x - PlayerPosition.x - MousePosition.y + PlayerPosition.y;
	float dir2 = MousePosition.x - PlayerPosition.x + MousePosition.y - PlayerPosition.y;
	if (dir1 < 0 && dir2>0)
		BubbleGunPosition = Vec2(5, 7);
	else if (dir1*dir2 >= 0)
		BubbleGunPosition = Vec2(7, -7);
	else if (dir1 > 0 && dir2 < 0)
		BubbleGunPosition = Vec2(5, -11);

	int K, J;
	if (MousePosition.x > PlayerPosition.x)K = 1;
	else K = -1;
	if ((MousePosition.x - (PlayerPosition.x + K * BubbleGunPosition.x)) < 0)J = 1;
	else J = -1;

	auto Theta = atanf((MousePosition.y - PlayerPosition.y - BubbleGunPosition.y) / (MousePosition.x - (PlayerPosition.x + K * BubbleGunPosition.x)));
	MyWeapon->setRotation((-PI / 2 - J * K*PI / 2 - K * Theta) / PI * 180);;
	MyWeapon->setOpacity(255);
	MyWeapon->setPosition(BubbleGunPosition);
	MyWeapon->setAnchorPoint(Vec2(0.50000, 0.30000));//once

	auto Bubble = Sprite::create("Bubble.png");
	auto BubblePosition = Vec2(PlayerPosition.x + K * BubbleGunPosition.x, PlayerPosition.y + BubbleGunPosition.y) + Vec2(35.69* cos(PI / 2 + J * PI / 2 + Theta + K * 0.197), 35.69* sin(PI / 2 + J * PI / 2 + Theta + K * 0.197));

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
	map->addChild(Bubble);

	Bubble->setName(player->getName());
	attackmove->setTag(2);
	Bubble->setTag(2);
	scene->Bullets->addObject(Bubble);
	Bubble->setZOrder(1);
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
	boom->setZOrder(2);
	auto map = who->getParent();

	if (map)map->addChild(boom);
	disappear(who);
}
Sprite* boomerang;
void Weapon::Boomerang(Point MousePosition)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("audio/Bubblegun.wav", false);

	auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();
	auto scene = (GameScene*)map->getParent();
	player->AttackMode1(MousePosition);
	auto PlayerPosition = player->getPosition();
	player->IsHaveWeapon = 0;
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
	map->addChild(boomerang);
	player->schedule(schedule_selector(Weapon::BoomerangBack), 2.0f / 60);

	boomerang->setName(player->getName());
	boomerang->setTag(3);
	scene->Bullets->addObject(boomerang);
	boomerang->setZOrder(1);
}
void Weapon::BoomerangBack(float dt)
{
	if (boomerang->numberOfRunningActions() == 1)
	{
		auto player = getMyplayer("Player");
		auto PlayerPosition = player->getPosition();
		auto BoomerangPosition = boomerang->getPosition();
		double l = sqrt((PlayerPosition.x - BoomerangPosition.x)*(PlayerPosition.x - BoomerangPosition.x) + (PlayerPosition.y - BoomerangPosition.y)*(PlayerPosition.y - BoomerangPosition.y));
		if (l <= 30)
		{
			player->IsHaveWeapon = 1;
			disappear(boomerang);
			player->unschedule(schedule_selector(Weapon::BoomerangBack));
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
void Weapon::Lance(Point MousePosition)
{
	auto player = (Player*)this->MyWeapon->getParent();
	auto map = (TMXTiledMap*)player->getParent();
	auto scene = (GameScene*)map->getParent();
	auto CharacterPosition = player->getPosition();
	if (sqrt((CharacterPosition.x - MousePosition.x)*(CharacterPosition.x - MousePosition.x) + (CharacterPosition.y - MousePosition.y)*(CharacterPosition.y - MousePosition.y)) <= 84)
	{
		player->AttackMode2(MousePosition);
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
		MyWeapon->setRotation(-90);//once
		MyWeapon->runAction(rotate);
		MyWeapon->runAction(seq);
	}
	else
	{
		player->AttackMode1(MousePosition);
		MyWeapon->stopAllActionsByTag(10);
		Point BowPosition;
		float dir1 = MousePosition.x - CharacterPosition.x - MousePosition.y + CharacterPosition.y;
		float dir2 = MousePosition.x - CharacterPosition.x + MousePosition.y - CharacterPosition.y;
		if (dir1 < 0 && dir2>0)
			BowPosition = Vec2(5, 7);
		else if (dir1*dir2 >= 0)
			BowPosition = Vec2(7, -7);
		else if (dir1 > 0 && dir2 < 0)
			BowPosition = Vec2(5, -11);

		int K, J;
		if (MousePosition.x > CharacterPosition.x)	K = 1;
		else K = -1;
		if ((MousePosition.x - (CharacterPosition.x + K * BowPosition.x)) < 0)J = 1;
		else J = -1;

		auto Theta = atanf((MousePosition.y - CharacterPosition.y - BowPosition.y) / (MousePosition.x - (CharacterPosition.x + K * BowPosition.x)));
		MyWeapon->setRotation((-PI / 4 - J * K*PI / 2 - K * Theta) / PI * 180);;
		MyWeapon->setPosition(BowPosition);
		MyWeapon->setOpacity(255);

		auto ArrowPosition = Vec2(CharacterPosition.x + K * BowPosition.x, CharacterPosition.y + BowPosition.y);
		double range = 84;
		float x = range * cos(PI / 2 + J * PI / 2 + Theta);// +ArrowPosition.x;
		float y = range * sin(PI / 2 + J * PI / 2 + Theta);// +ArrowPosition.y;
		auto move = MoveBy::create(3.0*WeaponSpeed[4] / 10, Vec2(K*x, y));
		auto back = CallFuncN::create(this, callfuncN_selector(Weapon::WeaponBack));
		auto seq = Sequence::create(move, move->reverse(), back, nullptr);
		MyWeapon->runAction(seq);
	}
}
void Weapon::disappear(Node* who)
{
	who->removeFromParentAndCleanup(TRUE);
}
void Weapon::WeaponBack(Node* who)
{
	auto player = (Player*)this->MyWeapon->getParent();
	player->IsHaveWeapon = 1;
}