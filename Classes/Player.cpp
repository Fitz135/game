#include"Player.h"
#include"GameScene.h"
//#include"HelloWorldScene.h"
#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2

Player::Player(std::string name, int id) {
	attr.hp = 100;
	attr.speed = 1;
	attr.attack = 30;
	attr.level = 1;
	attr.exp = 0;
	attr.weapon = 1;
	attr.superPower = 0;

	m_id = id;
	m_name = name;
}
Player* Player::getMychara(char* str) {
	return dynamic_cast<Player*>(GameScene::getCurrentMap()->getChildByName(str));
}
Player* Player::create(std::string name,int id)
{
	Player* player = new Player(name, id);
	if (player && player->initWithPlayerType())
	{	
		player->setName("Player");
		player->Legs->setPosition(0, 0);
		player->Body->setPosition(0,16);
		player->Head->setPosition(0,15);
		player->addChild(player->Legs);
		player->addChild(player->Body);
		player->addChild(player->Head);
		player->autorelease();
		return player;
	}
	else
	{
		delete player;
		player = NULL;
		return NULL;
	}
}
bool Player::initWithPlayerType()
{
	Legs = Sprite::createWithSpriteFrameName("Legs-0.png");
	Body = Sprite::createWithSpriteFrameName("Body-0.png");
	Head = Sprite::createWithSpriteFrameName("Head-0.png");
   
	MoveFrames[HEADMOVE]=AnimationFrames("Head-%d.png", 6, 19);
	MoveFrames[BODYMOVE]=AnimationFrames("Body-%d.png", 6, 19);
	MoveFrames[LEGSMOVE]=AnimationFrames("Legs-%d.png", 6, 19);

	AttackAbleFlag = 1;
	AttackEndFlag = 1;

	return true;
}
Vector<SpriteFrame*> Player::AnimationFrames(const char* FrameName, int begin, int end)
{
	auto spritecache = SpriteFrameCache::getInstance();
	char str[100];
	Vector<SpriteFrame*> animFrames;
	for (int i = begin; i <= end; i++)
	{
		sprintf(str, FrameName, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}
Animate* Player::createMoveAni(int i)
{
	auto MoveAnimation=Animation::createWithSpriteFrames(MoveFrames[i], 1.0f / 8);
	return Animate::create(MoveAnimation);
}
/*Animate* Character::createAttackAni()
{
	AttackFrames = AnimationFrames("Body-%d", 1, 3);
	auto AttackAnimation = Animation::createWithSpriteFrames(AttackFrames, 1.0f / 8);
	return Animate::create(AttackAnimation);
}*/
void Player::MoveBegin()
{
	auto LegsAni = RepeatForever::create(createMoveAni(LEGSMOVE));
	LegsAni->setTag(LEGSMOVE);
	Legs->runAction(LegsAni);

	if (AttackEndFlag)
	{
		auto HeadAni = RepeatForever::create(createMoveAni(HEADMOVE));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		auto BodyAni = RepeatForever::create(createMoveAni(BODYMOVE));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);
	}
}
void Player::MoveEnd()
{
	Legs->stopActionByTag(LEGSMOVE);
	Legs->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Legs-0.png"));

	if (AttackEndFlag)
	{
		Head->stopActionByTag(HEADMOVE);
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Head-0.png"));

		Body->stopActionByTag(BODYMOVE);
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Body-0.png"));
	}
}
void Player::AttackAbleflag(float dt)
{
	AttackAbleFlag = 1;
}
void Player::AttackEndflag(float dt)
{
	AttackEndFlag = 1;
}
void Player::AttackEnd(int pressnum)
{
	if (pressnum)
	{
		auto HeadAni = RepeatForever::create(createMoveAni(HEADMOVE));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		auto BodyAni = RepeatForever::create(createMoveAni(BODYMOVE));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);
	}
	else
	{
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Head-0.png"));
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Body-0.png"));
	}
}
void Player::Attack_Shoot(Point TouchPosition)
{
	if (AttackAbleFlag)
	{
		AttackAbleFlag = 0;
		AttackEndFlag = 0;

		this->unschedule(schedule_selector(Player::AttackAbleflag));
		this->schedule(schedule_selector(Player::AttackAbleflag), 1);
		this->scheduleOnce(schedule_selector(Player::AttackEndflag), 1);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		auto PlayerPosition = Body->getPosition();

		if (TouchPosition.x < PlayerPosition.x)
		{
			Legs->setFlipX(true);
			Body->setFlipX(true);
			Head->setFlipX(true);
		}
		else
		{
			Legs->setFlipX(false);
			Body->setFlipX(false);
			Head->setFlipX(false);
		}

		auto bullet = Sprite::create("Bullet.png");
		bullet->setPosition(PlayerPosition.x, PlayerPosition.y);
		auto scene= GameScene::getCurrentMap();
		scene->addChild(bullet);

		//projects->addObject(bullet);

		//bullet->setTag(2);

		double range = 300;
		double l = sqrt((TouchPosition.x - PlayerPosition.x)*(TouchPosition.x - PlayerPosition.x) + (TouchPosition.y - PlayerPosition.y)*(TouchPosition.y - PlayerPosition.y));
		double x = range * (TouchPosition.x - PlayerPosition.x) / l + PlayerPosition.x;
		double y = range * (TouchPosition.y - PlayerPosition.y) / l + PlayerPosition.y;

		auto move = MoveTo::create(3, Vec2(x, y));
		//auto disappear = CallFuncN::create(this, callfuncN_selector(MainScene::disappear));
		//auto attackmove = Sequence::create(move, disappear, NULL);

		bullet->runAction(move);

		float dir1 = TouchPosition.x - PlayerPosition.x - TouchPosition.y + PlayerPosition.y;
		float dir2 = TouchPosition.x - PlayerPosition.x + TouchPosition.y - PlayerPosition.y;

		Head->stopActionByTag(HEADMOVE);
		Body->stopActionByTag(BODYMOVE);
		if (dir1*dir2 >= 0)
		{
			Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Head-3.png"));
			Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Body-3.png"));
		}
		else if (dir1 < 0 && dir2>0)
		{
			Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Head-2.png"));
			Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Body-2.png"));
		}
		else if (dir1 > 0 && dir2 < 0)
		{
			Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Head-4.png"));
			Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Body-4.png"));
		}

	}
	return;
}

int Player::getId() {
	return m_id;
}
std::string Player::getName() {
	return m_name;
}