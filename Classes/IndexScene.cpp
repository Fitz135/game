#include"IndexScene.h"
#include"LoginScene.h"
USING_NS_CC;//using namespace cocos2d;

Scene* Index::createScene() {
	auto scene = Scene::create();
	auto layer=Index::create();
	layer->setTag(1);
	scene->addChild(layer);
	return scene;
}

bool Index::init() {
	auto labelStart = Label::create("Start", "arial.ttf", 30);
	auto labelExit = Label::create("Exit", "arial", 30);
	auto itemStart = MenuItemLabel::create(labelStart, CC_CALLBACK_1( Index::startCallback,this));
	auto itemExit = MenuItemLabel::create(labelExit, CC_CALLBACK_1(Index::exitCallback,this));
	auto menu = Menu::create();
	menu->addChild(itemStart);
	menu->addChild(itemExit);
	menu->alignItemsVerticallyWithPadding(20);
	this->addChild(menu);

	for (int i = 1; i <= 7; i++)
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Player/" + std::to_string(i) + ".plist");
	return true;
}
void Index::startCallback(Ref* ref) {
	auto layer = LoginScene::create();
	layer->setTag(1);
	this->addChild(layer);
}
void Index::exitCallback(Ref* ref) {
	Director::getInstance()->end();
}