#include "NormalScene.h"
#include "Player.h"
USING_NS_CC;

cocos2d::Scene* Normal::createScene() {
	return Normal::create();
}

bool Normal::init() {

	Player* player = Player::create("local");
	player->addPlayer();

	player->setTag(1);
	player->setPosition(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height / 2);
	this->addChild(player);
	/*SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Player/KnightStand.plist");
	auto am = CCAnimation::create();
	for (int i = 0; i < 4; i++) {
		CCString* str = CCString::createWithFormat("%d", i);
		am->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString()));
	}
	am->setDelayPerUnit(0.2);
	am->setLoops(-1);
	auto ame = CCAnimate::create(am);
	player->runAction(ame);*/

	/*auto am = CCAnimation::create();
	am->addSpriteFrameWithFileName("E:\cocosTest\Resources\Player\KnightStand1.png");
	am->addSpriteFrameWithFileName("E:\cocosTest\Resources\Player\KnightStand2.png");
	am->addSpriteFrameWithFileName("E:\cocosTest\Resources\Player\KnightStand3.png");
	am->addSpriteFrameWithFileName("E:\cocosTest\Resources\Player\KnightStand4.png");
	am->setDelayPerUnit(0.16f);
	am->setLoops(-1);
	auto ame = CCAnimate::create(am);
	player->runAction(ame);*/
	/*
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(4);
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand1.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand2.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand3.png", Rect(0, 0, 340, 425)));
	animFrames.pushBack(SpriteFrame::create("Player/KnightStand4.png", Rect(0, 0, 340, 425)));
	auto am = Animation::createWithSpriteFrames(animFrames, 1.0f);
	auto ame = Animate::create(am);
	player->runAction(ame);*/
	auto keyboardLs = EventListenerKeyboard::create();
	keyboardLs->onKeyPressed = [=](EventKeyboard::KeyCode code,Event* event) {
		if (code == EventKeyboard::KeyCode::KEY_A) {
			this->schedule(schedule_selector(Player::moveLeft), 1/60);
		}
		if (code == EventKeyboard::KeyCode::KEY_D) {
			this->schedule(schedule_selector(Player::moveRight), 1 / 60);
		}
		if (code == EventKeyboard::KeyCode::KEY_W) {
			this->schedule(schedule_selector(Player::moveUp), 1 / 60);
		}
		if (code == EventKeyboard::KeyCode::KEY_S) {
			this->schedule(schedule_selector(Player::moveDown), 1 / 60);
		}
	};
	keyboardLs->onKeyReleased= [=](EventKeyboard::KeyCode code, Event* event) {
		if (code == EventKeyboard::KeyCode::KEY_A) {
			this->unschedule(schedule_selector(Player::moveLeft));
		}
		if (code == EventKeyboard::KeyCode::KEY_D) {
			this->unschedule(schedule_selector(Player::moveRight));
		}
		if (code == EventKeyboard::KeyCode::KEY_W) {
			this->unschedule(schedule_selector(Player::moveUp));
		}
		if (code == EventKeyboard::KeyCode::KEY_S) {
			this->unschedule(schedule_selector(Player::moveDown));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardLs, this);
	return true;
}
