#include"Player.h"
#include"GameScene.h"
#include"Weapon.h"

#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2
#define BODYATTACK   3
#define HANDATTACK   4

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
	if (player && player->initWithPlayerType(id+1))
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
	this->CharaType = i;
	Legs = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Legs/Legs-0.png");
	Body = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Body/Body-0.png");
	Head = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Head/Head-0.png");
	Hand = Sprite::createWithSpriteFrameName("Player" + std::to_string(CharaType) + "/Arm/Arm-0.png");
	weapon = Weapon::create();

	Legs->setPosition(0, -14);
	Head->setPosition(0, -1);

	addChild(Head);
	addChild(Body);
	addChild(Legs);
	addChild(Hand);

	Hand->setOpacity(0);
	Hand->setZOrder(2);

	MoveFrames[HEADMOVE] = AnimationFrames("Player" + std::to_string(CharaType) + "/Head/Head-", 6, 19);
	MoveFrames[BODYMOVE] = AnimationFrames("Player" + std::to_string(CharaType) + "/Body/Body-", 6, 19);
	MoveFrames[LEGSMOVE] = AnimationFrames("Player" + std::to_string(CharaType) + "/Legs/Legs-", 6, 19);
	MoveFrames[BODYATTACK] = AnimationFrames("Player" + std::to_string(CharaType) + "/Body/Body-", 1, 4);
	MoveFrames[HANDATTACK] = AnimationFrames("Player" + std::to_string(CharaType) + "/Arm/Arm-", 1, 4);

	AttackAbleFlag = 1;
	AttackEndFlag = 1;
	IsHaveWeapon = 1;
	return true;
}
Weapon* Player::ChangeWeapon(int WeaponType)
{
	if (WeaponType == 1)
		this->AttackMode = &Player::AttackMode2;
	else
		this->AttackMode = &Player::AttackMode1;

	if (weapon)
	{
		weapon->MyWeapon->removeFromParentAndCleanup(TRUE);
		delete weapon;
	}
	weapon = Weapon::create(WeaponType);
	weapon->retain();
	AttackSpeed = weapon->WeaponSpeed[WeaponType];
	addChild(weapon->MyWeapon);
	return weapon;
}
Vector<SpriteFrame*> Player::AnimationFrames(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end)
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	for (int i = begin; i <= end; i++)
		animFrames.pushBack(spritecache->getSpriteFrameByName(FrameName + std::to_string(i) + ".png"));
	return animFrames;
}
Animate* Player::createAnimate(int FramesIndex, float delay)
{
	auto MoveAnimation = Animation::createWithSpriteFrames(MoveFrames[FramesIndex], delay);
	return Animate::create(MoveAnimation);
}

void Player::MoveBegin()
{
	auto LegsAni = RepeatForever::create(createAnimate(LEGSMOVE, 1.0f / 16));
	LegsAni->setTag(LEGSMOVE);
	Legs->runAction(LegsAni);

	if (AttackEndFlag)
	{
		auto HeadAni = RepeatForever::create(createAnimate(HEADMOVE, 1.0f / 16));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		auto BodyAni = RepeatForever::create(createAnimate(BODYMOVE, 1.0f / 16));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);
	}
}
void Player::MoveEnd()
{
	Legs->stopActionByTag(LEGSMOVE);
	Legs->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Legs/Legs-0.png"));

	if (AttackEndFlag)
	{
		Head->stopActionByTag(HEADMOVE);
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));

		Body->stopActionByTag(BODYMOVE);
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-0.png"));
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
	Hand->setOpacity(0);
	weapon->MyWeapon->setOpacity(0);
	Body->stopActionByTag(BODYATTACK);
	Hand->stopActionByTag(HANDATTACK);
	if (pressnum)
	{
		auto HeadAni = RepeatForever::create(createAnimate(HEADMOVE, 1.0f / 16));
		HeadAni->setTag(HEADMOVE);
		Head->runAction(HeadAni);

		auto BodyAni = RepeatForever::create(createAnimate(BODYMOVE, 1.0f / 16));
		BodyAni->setTag(BODYMOVE);
		Body->runAction(BodyAni);
	}
	else
	{
		Head->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));
		Body->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Body/Body-0.png"));
	}
}
void Player::AttackBegan(Point TouchPosition)
{
	AttackAbleFlag = 0;
	AttackEndFlag = 0;
	this->unschedule(schedule_selector(Player::AttackAbleflag));
	this->schedule(schedule_selector(Player::AttackAbleflag), AttackSpeed);
	this->scheduleOnce(schedule_selector(Player::AttackEndflag), AttackSpeed);
	(weapon->*(weapon->WeaponMode))(TouchPosition);
	(this->*(this->AttackMode))(TouchPosition);
	return;
}
void Player::AttackMode1(Point TouchPosition)
{
	auto Player = getMychara("Player");
	auto PlayerPosition = Player->getPosition();
	float dir1 = TouchPosition.x - PlayerPosition.x - TouchPosition.y + PlayerPosition.y;
	float dir2 = TouchPosition.x - PlayerPosition.x + TouchPosition.y - PlayerPosition.y;

	Head->stopActionByTag(HEADMOVE);
	Body->stopActionByTag(BODYMOVE);
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
}

void Player::AttackMode2(Point TouchPosition)
{
	Hand->setOpacity(255);
	Head->stopActionByTag(HEADMOVE);
	Head->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player" + std::to_string(CharaType) + "/Head/Head-0.png"));

	auto BodyAni = createAnimate(BODYATTACK, 1.0f / 8);
	BodyAni->setTag(BODYATTACK);
	Body->stopActionByTag(BODYMOVE);
	Body->runAction(BodyAni);

	auto HandAni = createAnimate(HANDATTACK, 1.0f / 8);
	HandAni->setTag(HANDATTACK);
	Hand->runAction(HandAni);
}
int Player::getId() {
	return m_id;
}
std::string Player::getName() {
	return m_name;
}