#include"IndexScene.h"
#include"GameModeScene.h"
USING_NS_CC;//using namespace cocos2d;

Scene* Index::createScene() {
	return Index::create();
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

	return true;
}
void Index::startCallback(Ref* ref) {
	auto scene = GameMode::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));//replaceScene( scene);
}
void Index::exitCallback(Ref* ref) {
	Director::getInstance()->end();
}