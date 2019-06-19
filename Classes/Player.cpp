#include"Player.h"
#include"GameScene.h"
#include"AiPlayer.h"
#include"Weapon.h"
#include"Settings.h"
#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2
#define BODYATTACK   3
#define HANDATTACK   4

Player::Player(std::string name, int id) :info(Entity(name, id)){
	attr.hp = 100;
	attr.speed = 1;
	attr.attack = 30;
	attr.level = 1;
	attr.exp = 0;
	attr.weapon = 1;
	attr.superPower = 0;
}
Player* Player::create(std::string name,int id)
{
	Player* player = new Player(name, id);
	if (player && player->initWithPlayerType(id))
	{	
		if(id==local_Id)
		player->setName("Player");
		player->setTag(id);
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
Player* Player::create(Entity* e)
{
	Player* player = new Player(e->getName(), e->getId());
	if (player && player->initWithPlayerType(e->getId() + 1))
	{
		player->setName("Player");
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
bool Player::initWithPlayerType(int i)
{
	CharaType = i;
	MoveSpeed = 8;
	weapon = static_cast<Weapon*>(nullptr);

	HPBar = Hp::create();
	HPBar->setName("HPBar");
	HPBar->setScale(0.5);
	HPBar->retain();

	Legs = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Legs/Legs-0.png");
	Body = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Body/Body-0.png");
	Head = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Head/Head-0.png");
	Hand = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Arm/Arm-0.png");

	Legs->setPosition(0, -14);
	Head->setPosition(0, -1);

	addChild(Head);
	addChild(Body);
	addChild(Legs);
	addChild(Hand,2);

	Hand->setOpacity(0);

	return true;
}
void Player::update(float dt)
{
	if (!weapon||weapon->getReferenceCount() == 1)
	{
		ChangeWeapon(WeaponType);
		this->unscheduleUpdate();
	}
}
void Player::ChangeWeapon(int weapontype)
{
	if (weapon)
	{
		weapon->MyWeapon->removeFromParentAndCleanup(TRUE);
		delete weapon;
	}
	weapon = Weapon::create(WeaponType);
	weapon->retain();
	IsHaveWeapon = 1;
	weapon->MyWeapon->setTag(WeaponType);
	AttackSpeed = weapon->WeaponSpeed[WeaponType];
	addChild(weapon->MyWeapon,1);
	return ;
}

Animate* Player::createAnimate(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end, float delay)
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	for (int i = begin; i <= end; i++)
		animFrames.pushBack(spritecache->getSpriteFrameByName(FrameName + std::to_string(i) + ".png"));
	auto MoveAnimation = Animation::createWithSpriteFrames(animFrames, delay);
	return Animate::create(MoveAnimation);
}

void Player::MoveBegin()
{
	Legs->stopAllActionsByTag(LEGSMOVE);
	auto LegsAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Legs/Legs-", 6, 19, 2.0f / (MoveSpeed * 8)));
	LegsAni->setTag(LEGSMOVE);
	Legs->runAction(LegsAni);
	if (AttackAbleFlag)
	{
		Head->stopAllActionsByTag(HEADMOVE);
		auto HeadAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Head/Head-", 6, 19, 2.0f / (MoveSpeed * 8)));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		Body->stopAllActionsByTag(BODYMOVE);
		Body->stopAllActionsByTag(BODYATTACK);
		auto BodyAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Body/Body-", 6, 19, 2.0f / (MoveSpeed * 8)));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);
	}
}
void Player::MoveEnd()
{
	Legs->stopAllActionsByTag(LEGSMOVE);
	Legs->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Legs/Legs-0.png"));

	if (AttackAbleFlag)
	{
		Head->stopAllActionsByTag(HEADMOVE);
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));

		Body->stopAllActionsByTag(BODYMOVE);
		Body->stopAllActionsByTag(BODYATTACK);
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-0.png"));
	}
}
void Player::AttackAbleflag(float dt)
{
	AttackAbleFlag = 1;
}

void Player::AttackEnd(int pressnum)
{
	Hand->setOpacity(0);
	MoveSpeed = 8;
	weapon->MyWeapon->setOpacity(0);
	Body->stopAllActionsByTag(BODYATTACK);
	Hand->stopAllActionsByTag(HANDATTACK);
	if (pressnum)
	{
		Head->stopAllActionsByTag(HEADMOVE);
		auto HeadAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Head/Head-", 6, 19, 2.0f / (MoveSpeed * 8)));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		Body->stopAllActionsByTag(BODYMOVE);
		auto BodyAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Body/Body-", 6, 19, 2.0f / (MoveSpeed * 8)));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);

		if (WeaponType == 5)
		{
			Legs->stopAllActionsByTag(LEGSMOVE);
			auto LegsAni = RepeatForever::create(createAnimate("Player" + std::to_string(CharaType) + "/Legs/Legs-", 6, 19, 2.0f / (MoveSpeed * 8)));
			LegsAni->setTag(LEGSMOVE);
			Legs->runAction(LegsAni);
		}
	}
	else
	{
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-0.png"));
		Legs->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Legs/Legs-0.png"));
	}
}
void Player::AttackBegan(Point TouchPosition)
{
	AttackAbleFlag = 0;

	auto map = dynamic_cast<TMXTiledMap *>(this->getParent());
	TouchPosition -= map->getPosition();
	if (WeaponType != 5)
	{
		if (TouchPosition.x < this->getPositionX())
			this->setScaleX(-1);
		else
			this->setScaleX(1);
	}
	if(weapon)
	(weapon->*(weapon->WeaponMode))(TouchPosition);
	return;
}
void Player::AttackMode1(Point TouchPosition)
{
	auto PlayerPosition = this->getPosition();
	float dir1 = TouchPosition.x - PlayerPosition.x - TouchPosition.y + PlayerPosition.y;
	float dir2 = TouchPosition.x - PlayerPosition.x + TouchPosition.y - PlayerPosition.y;

	Head->stopAllActionsByTag(HEADMOVE);
	Body->stopAllActionsByTag(BODYMOVE);
	Hand->setOpacity(255);
	if (dir1*dir2 >= 0)
	{
		Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));
		Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-3.png"));
		Hand->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Arm/Arm-3.png"));
	}
	else if (dir1 < 0 && dir2>0)
	{
		Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));
		Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-2.png"));
		Hand->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Arm/Arm-2.png"));
	}
	else if (dir1 > 0 && dir2 < 0)
	{
		Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));
		Body->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-4.png"));
		Hand->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Arm/Arm-4.png"));
	}
	this->scheduleOnce(schedule_selector(Player::AttackAbleflag), AttackSpeed );
}

void Player::AttackMode2(Point TouchPosition)
{
	Hand->setOpacity(255);
	Head->stopAllActionsByTag(HEADMOVE);
	Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));

	Body->stopAllActionsByTag(BODYATTACK);
	Body->stopAllActionsByTag(BODYMOVE);
	auto BodyAni = createAnimate("Player" + std::to_string(CharaType) + "/Body/Body-", 1, 4, AttackSpeed / 5);
	BodyAni->setTag(BODYATTACK);
	Body->runAction(BodyAni);

	Hand->stopAllActionsByTag(HANDATTACK);
	auto HandAni = createAnimate("Player" + std::to_string(CharaType) + "/Arm/Arm-", 1, 4, AttackSpeed / 5);
	HandAni->setTag(HANDATTACK);
	Hand->runAction(HandAni);
	this->scheduleOnce(schedule_selector(Player::AttackAbleflag), 6.0*AttackSpeed / 10);
}
void Player::AttackMode3(Point TouchPosition)
{
	Head->stopAllActionsByTag(HEADMOVE);
	Body->stopAllActionsByTag(BODYMOVE);
	Legs->stopAllActionsByTag(LEGSMOVE);

	auto HeadAni =createAnimate("Player" + std::to_string(CharaType) + "/Head/Head-", 6, 19, 2.0f / (MoveSpeed * 8));
	HeadAni->setTag(HEADMOVE);
	Head->runAction(HeadAni);

	auto BodyAni =createAnimate("Player" + std::to_string(CharaType) + "/Body/Body-", 6, 19, 2.0f / (MoveSpeed * 8));
	BodyAni->setTag(BODYMOVE);
	Body->runAction(BodyAni);

	auto LegsAni = createAnimate("Player" + std::to_string(CharaType) + "/Legs/Legs-", 6, 19, 2.0f / (MoveSpeed * 8));
	LegsAni->setTag(LEGSMOVE);
	Legs->runAction(LegsAni);

	this->scheduleOnce(schedule_selector(Player::AttackAbleflag), 13*2.0f / (MoveSpeed * 8));
}
void Player::Dead(Node* who)
{
	if(dynamic_cast<Player*>(who)->IsAI)who->getChildByName("aiop")->unschedule(schedule_selector(AiPlayer::AiAttack));
	else who->unscheduleAllSelectors();
	AttackAbleFlag = 0;
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	for (int i = 0; i <= 3; i++)
		animFrames.pushBack(spritecache->getSpriteFrameByName("ghost/ghost-" + std::to_string(i) + ".png"));
	auto dead =Animate::create(Animation::createWithSpriteFrames(animFrames, 1.0f/8));
	auto sp = Sprite::create();
	addChild(sp);
	Body->setOpacity(0);
	Head->setOpacity(0);
	Legs->setOpacity(0);
	Hand->setOpacity(0);
	HPBar->removeFromParentAndCleanup(true);
	if(weapon)
	weapon->MyWeapon->setOpacity(0);
	sp->setScaleX(-1);
	sp->setZOrder(100);
	sp->runAction(RepeatForever::create(dead));
}
int Player::getId() {
	return info._id;
}

/*std::string Player::getName() {
	return info._name;
}*/