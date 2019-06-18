#include"IndexScene.h"
#include"LoginScene.h"
#include"SimpleAudioEngine.h"

#include"ResultScene.h"

USING_NS_CC;//using namespace cocos2d;

Scene* Index::createScene() {
	auto scene = Scene::create();
	auto layer=Index::create();
	layer->setTag(1);
	scene->addChild(layer);
	
	return scene;
}

bool Index::init() {
	auto labelStart = Label::create("Start", "fonts/Cordelia.ttf", 60);
	auto labelExit = Label::create("Exit", "fonts/Cordelia.ttf", 60);
	auto itemStart = MenuItemLabel::create(labelStart, CC_CALLBACK_1( Index::startCallback,this));
	auto itemExit = MenuItemLabel::create(labelExit, CC_CALLBACK_1(Index::exitCallback,this));
	auto menu = Menu::create();
	menu->addChild(itemStart);
	menu->addChild(itemExit);
	menu->alignItemsVerticallyWithPadding(40);
	//menu->setScale(2);
	this->addChild(menu);

	for (int i = 1; i <= 7; i++)
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Player/" + std::to_string(i) + ".plist");
	//schedule(schedule_selector(Index::test), 0.1);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Bullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("death.plist");




	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("audio/bgm1.wav");
	

	return true;
}
void Index::startCallback(Ref* ref) {
	auto layer = LoginScene::create();
	layer->setTag(1);
	this->addChild(layer);
	/*auto layer = ResultScene::create();
	layer->setTag(1);
	this->addChild(layer);*/
}
void Index::exitCallback(Ref* ref) {
	Director::getInstance()->end();
}
void Index::onEnter() {
	Layer::onEnter();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("audio/bgm1.wav", true);
}