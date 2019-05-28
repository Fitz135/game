#include"GameModeScene.h"
#include"NormalScene.h"
USING_NS_CC;

Scene* GameMode::createScene() {
	return GameMode::create();
}

bool GameMode::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//
	auto labelNormal = Label::create("Normal", "arial.ttf", 30);
	auto labelCrazy = Label::create("Crazy", "arial.ttf", 30);
	auto labelEndless = Label::create("Endless", "arial.ttf", 30);
	auto itemNormal = MenuItemLabel::create(labelNormal, CC_CALLBACK_1(GameMode::gamestartCallback,this));
	auto itemCrazy = MenuItemLabel::create(labelCrazy, CC_CALLBACK_1(GameMode::gamestartCallback,this) );
	auto itemEndless = MenuItemLabel::create(labelEndless, CC_CALLBACK_1(GameMode::gamestartCallback,this));
	auto menu = Menu::create();
	menu->addChild(itemCrazy);
	menu->addChild(itemNormal);
	menu->addChild(itemEndless);
	
	auto labelBack = Label::create("Back", "arial.ttf", 30);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1( GameMode::menubackCallback,this));
	//itemBack->setPosition(labelBack->getContentSize() .width/ 2, visibleSize.height - itemBack->getContentSize().height / 2);
	menu->alignItemsHorizontallyWithPadding(10);
	menu->addChild(itemBack);

	this->addChild(menu);
	//
	return true;
}
void GameMode::gamestartCallback(Ref* ref) {
	auto scene = Normal::createScene();
	Director::getInstance()->setClearColor(Color4F::WHITE);
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
};
void GameMode::menubackCallback(Ref* ref) {
	Director::getInstance()->popScene();
}