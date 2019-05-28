#include"Player.h"
USING_NS_CC;
Player::Player() {

	attr->hp= 100;
	attr->speed= 1;
    attr->attack= 30;
	attr->level = 1;
	attr->exp = 0;
    attr->weapon=1;
    attr->superPower=0;
	sprite = Sprite::create("Player/KnightStand1.png");
}
Player::~Player()
{
}
Player* Player::create(const std::string& id, int role)
{
	auto player = new (std::nothrow) Player();
	if (player&&player->init())
    {
        player->_id=id;

		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);

	return nullptr;
}

void Player::addPlayer()
{

	this->removeChild(sprite, true);
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(4);
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand1.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand2.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand3.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand4.png", Rect(0, 0, 340, 425)));
	auto am = Animation::createWithSpriteFrames(animFrames, 0.161f);
	auto ame = Animate::create(am);
	sprite->runAction(RepeatForever::create(ame));
	this->addChild(sprite);
}


 bool Player::init()
{
    direction=Direction::NONE;
                                //map
	return true;
}

void Player::setSpeed(int Speed)
{
    this->attr.speed=speed;
}
uint8_t Player::getSpeed()
{
	return attr.speed;
}
float Player::getRealSpeed()
{
    //
}


uint8_t getDamage()
{
    this->attr.
}
void setDamage(uint8_t damage)
{
    
}

bool isAlive()
{
    return hp>=0;
}

uint8_t getWeapon()
{
    return attr.weapon;
}
void setWeapon(uint8_t newWeapon)
{
    attr.weapon=newWeapon;
}

void attack()
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

