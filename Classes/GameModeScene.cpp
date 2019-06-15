#include"GameModeScene.h"
//#include"NormalScene.h"
#include"GameScene.h"
#include"RoomScene.h"
#include"Settings.h"

USING_NS_CC;

Scene* GameMode::createScene() {
	return GameMode::create();
}

bool GameMode::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//
	auto labelNormal = Label::create("PvP", "fonts/arial.ttf", 30);
	auto labelCrazy = Label::create("PvE", "fonts/arial.ttf", 30);	
	auto labelBack = Label::create("Back", "fonts/arial.ttf", 30);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1( GameMode::menubackCallback,this));
	auto itemNormal = MenuItemLabel::create(labelNormal, CC_CALLBACK_1(GameMode::gamestartCallback,this));
	auto itemCrazy = MenuItemLabel::create(labelCrazy, CC_CALLBACK_1(GameMode::gamestartCallback,this) );
	auto menu = Menu::create();
	menu->addChild(itemCrazy);
	menu->addChild(itemNormal);

	menu->alignItemsHorizontallyWithPadding(20);
	itemBack->setPosition(-visibleSize.width / 2+labelBack->getContentSize() .width/ 2+5, visibleSize.height/2 - itemBack->getContentSize().height / 2);
	menu->addChild(itemBack);
	this->addChild(menu);
	
	createBG();
	
	auto labelId= Label::create(local_username, "fonts/arial.ttf", 30);//"123"
	labelId->setPosition(visibleSize.width - labelId->getContentSize().width / 2, visibleSize.height - labelId->getContentSize().height / 2);
	this->addChild(labelId);
	return true;
}
void GameMode::gamestartCallback(Ref* ref) {

	/*auto bg = this->getChildByTag(1);
	bg->setColor(Color3B(100, 100, 100));
	auto createroom = CreateRoom::create();
	this->addChild(createroom);*/

	//Director::getInstance()->setClearColor(Color4F::WHITE);

	auto scene = RoomScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
};
void GameMode::menubackCallback(Ref* ref) {
	Director::getInstance()->popScene();
}
void GameMode::createBG() {
	auto imgBG = Sprite::create("UI/GameModeBG.png");
	if (imgBG != nullptr) {
		auto x = Director::getInstance()->getVisibleSize().width / 2;
		auto y = Director::getInstance()->getVisibleSize().height / 2;
		imgBG->setPosition(x, y);
		imgBG->setScale(1.2);
		imgBG->setTag(1);
		this->addChild(imgBG, -1);
		log("BG succ");
	}
	else
	{
		log("BG fail");
	}
}