#include"Player.h"
USING_NS_CC;
Player::Player() {

	attr.hp= 100;
	attr.speed= 1;
    attr.attack= 30;
	attr.level = 1;
	attr.exp = 0;
    attr.weapon=1;
    attr.superPower=0;
	sprite = Sprite::create("Player/KnightStand1.png");
}
Player::~Player()
{

}
Player* Player::create(const std::string& id)
{
	auto player = new (std::nothrow) Player();
	if (player&&player->init())
    {
        player->id=id;
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);//可能要放在dtor
	
	return nullptr;
}

void Player::addPlayer()
{

	this->removeChild(sprite, true);
	initAnimation();
	this->addChild(sprite);
}

bool Player::initAnimation() {
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(4);
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand1.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand2.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand3.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand4.png", Rect(0, 0, 340, 425)));
	auto am = Animation::createWithSpriteFrames(animFrames, 0.161f);
	if (am!=nullptr) {
		auto ame = Animate::create(am);
		sprite->runAction(RepeatForever::create(ame));
		return true;
	}
	else return false;
}

 bool Player::init()
{
    direction=Direction::UP;
                                //map
	return true;
}

void Player::setSpeed(uint8_t Speed)
{
    attr.speed=Speed;
}
uint8_t Player::getSpeed()
{
	return attr.speed;
}



uint8_t Player::getDamage()
{
	return attr.attack;
}
void Player::setDamage(uint8_t damage)
{
    
}

bool Player::isAlive()
{
    return attr.hp>=0;
}

uint8_t Player::getWeapon()
{
    return attr.weapon;
}
void Player::setWeapon(uint8_t newWeapon)
{
    attr.weapon=newWeapon;
}

void Player::attack()
{
    
}

void Player::moveLeft(float f)
{

	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int leftBound = player->getContentSize().width / 2;
	if (point.x >= leftBound)
		player->setPosition(point.x - 1, point.y);
}

void Player::moveRight(float f)
{

	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int rightBound = Director::getInstance()->getVisibleSize().width - player->getContentSize().width / 2;
	if (point.x <= rightBound)
		player->setPosition(point.x + 1, point.y);
}

void Player::moveUp(float f)
{
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int upBound = Director::getInstance()->getVisibleSize().height - player->getContentSize().height / 2;
	if (point.y <= upBound)
		player->setPosition(point.x, point.y + 1);
}

void Player::moveDown(float f)
{
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int downBound = player->getContentSize().height / 2;
	if (point.y >= downBound)
		player->setPosition(point.x, point.y - 1);
}

