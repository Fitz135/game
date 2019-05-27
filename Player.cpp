#include"Player.h"
USING_NS_CC;
Player::Player() {
	hp = 100;
	speed = 1;
	atk = 30;
	def = 10;
	level = 1;
	exp = 0;
	goal = 0;
	//Weapon
	sprite = Sprite::create("Player/KnightStand1.png");
}
Player::~Player() {

}
Player* Player::create() {
	Player* player = new Player();
	if (player&&player->init()) {
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return NULL;
}
void Player::addPlayer() {
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
 bool Player::init() {
	return true;
}
void Player::setSpeed(int Speed) {
	speed = Speed;
}
int Player::getSpeed() {
	return speed;
}
void Player::setId(int Id) {
	id = Id;
}
int Player::getId() {
	return id;
}
void Player::moveLeft(float f) {
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int leftBound = player->getContentSize().width / 2;
	if (point.x >= leftBound)
		player->setPosition(point.x - 1, point.y);
}
void Player::moveRight(float f) {
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int rightBound = Director::getInstance()->getVisibleSize().width - player->getContentSize().width / 2;
	if (point.x <= rightBound)
		player->setPosition(point.x + 1, point.y);
}
void Player::moveUp(float f) {
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int upBound = Director::getInstance()->getVisibleSize().height - player->getContentSize().height / 2;
	if (point.y <= upBound)
		player->setPosition(point.x, point.y + 1);
}
void Player::moveDown(float f) {
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	auto point = player->getPosition();
	int downBound = player->getContentSize().height / 2;
	if (point.y >= downBound)
		player->setPosition(point.x, point.y - 1);
}