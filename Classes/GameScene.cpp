#include"GameScene.h"
#include "Player.h"
#include "Operator.h"
USING_NS_CC;
	
Scene* GameScene::createScene() {
	auto scene = Scene::create();

	auto layer = GameScene::create();

	layer->setName("GameScene");
	layer->setTag(10);

	scene->addChild(layer);

	return scene;
}

Scene* GameScene::getCurrentMap() {
	/*auto currentScene = Director::getInstance()->getRunningScene();
	return dynamic_cast<GameScene*>(currentScene->getChildByName("GameScene"));*/
	return Director::getInstance()->getRunningScene();
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Test.plist");
	auto player=Player::create("123", 1);
	this->addChild(player);

	auto operate = Operator::create();
	addChild(operate);
	return true;
}
