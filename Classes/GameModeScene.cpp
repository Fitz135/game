#include"GameModeScene.h"
//#include"NormalScene.h"
#include"GameScene.h"
#include"RoomScene.h"
#include"Settings.h"
#include"IPScene.h"
#include"HP.h"
USING_NS_CC;

Scene* GameMode::createScene() {
	return GameMode::create();
}

bool GameMode::init() {
	auto visibleSize = Director::getInstance()->getWinSize();
	//
	auto labelPvP = Label::create("PvP", "fonts/Cordelia.ttf", 60);
	auto labelPvE = Label::create("PvE", "fonts/Cordelia.ttf", 60);	
	auto labelBack = Label::create("Back", "fonts/Cordelia.ttf", 60);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1( GameMode::menubackCallback,this));
	auto itemPvP = MenuItemLabel::create(labelPvP, CC_CALLBACK_1(GameMode::gamestartCallback,this));
	auto itemPvE = MenuItemLabel::create(labelPvE, CC_CALLBACK_1(GameMode::gamestartCallback,this) );
	auto itemIP = MenuItemLabel::create(Label::create("IP", "fonts/Cordelia.ttf", 60) , CC_CALLBACK_1(GameMode::gamestartCallback, this));
	itemIP->setScale(0.8);

	
	itemPvP->setScale(1.3);
	itemPvE->setScale(1.3);
	auto menu = Menu::create();
	menu->addChild(itemPvP);
	
	menu->addChild(itemPvE);

	menu->alignItemsVerticallyWithPadding(30);
	itemBack->setPosition(-visibleSize.width*0.4, visibleSize.height*0.15);
	itemIP->setPosition(visibleSize.width*0.25, -visibleSize.height*0.33);
	itemBack->setScale(0.8);
	menu->addChild(itemIP);
	menu->addChild(itemBack);
	menu->setRotation(-9);
	menu->setPosition(visibleSize.width/2-visibleSize.width*0.1 ,visibleSize.height / 2 +visibleSize.height*0.155);
	menu->setColor(Color3B(143, 137, 137));
	this->addChild(menu);
	
	createBG();
	
	auto labelId= Label::create(local_username, "fonts/Marker Felt.ttf", 40);
	labelId->setPosition(visibleSize.width*0.85 , visibleSize.height*0.715 );
	labelId->setRotation(-10);
	this->addChild(labelId);

	/*auto hp = Hp::create();
	hp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	hp->setHp(88);
	addChild(hp);*/

	
	return true;
}
void GameMode::gamestartCallback(Ref* ref) {

	
	MenuItemLabel* item = (MenuItemLabel*)ref;
	if (item->getString() == "PvP") {
		gameMode = false;
		roomscene = RoomScene::create();
		Director::getInstance()->pushScene(TransitionFade::create(0.5, roomscene));
	}
	else if (item->getString() == "PvE") {
		gameMode = true;
		auto scene = GameScene::createScene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
	}
	else if (item->getString() == "IP") {
		auto cl = LayerColor::create(Color4B::BLACK);
		cl->setOpacity(150);
		cl->setName("solid");
		addChild(cl,1);

		auto ip = IPScene::create();
		ip->setTag(1);
		this->addChild(ip, 2);
	}
	
};
void GameMode::menubackCallback(Ref* ref) {
	Director::getInstance()->popScene();
}
void GameMode::createBG() {
	auto imgBG = Sprite::create("UI/GameModeBG.png");
	if (imgBG != nullptr) {
		auto x = Director::getInstance()->getWinSize().width / 2;
		auto y = Director::getInstance()->getWinSize().height / 2;
		imgBG->setPosition(x, y);
		imgBG->setScaleX(1.28);
		imgBG->setScaleY(1.45);
		imgBG->setName("bg");
		this->addChild(imgBG, -1);
		//log("BG succ");
	}
	else
	{
		//log("BG fail");
	}
}