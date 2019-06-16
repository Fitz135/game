#include"LoginScene.h"
#include"GameModeScene.h"
#include"Settings.h"
USING_NS_CC;	

bool LoginScene::init() {
	if (!Layer::init()) {
		return false;
	}
	
	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;

	bg = Sprite::create("UI/LoginBG.png");
	bg->setScale(0.55);
	bg->setPosition(center_x, center_y);

	id = ui::TextField::create("12 character most ", "fonts/arial.ttf", 25);
	//auto idBG = Sprite::create("UI/TextFieldBG.png");
	id->setPosition(Vec2(center_x, center_y*1.1));
	id->setColor(Color3B(150,150,150));
	id->setTag(1);
	id->setCursorEnabled(true);
	id->setCursorChar('|');
	id->setMaxLength(12);
	//idBG->setPosition(center_x, center_y);

	auto finish = MenuItemLabel::create(Label::create("Finish", "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(LoginScene::startCallback, this));
	auto canel = MenuItemLabel::create(Label::create("Cancel", "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(LoginScene::startCallback, this));
	auto menu = Menu::create();
	menu->setPosition(center_x, center_y * 2 / 3);
	menu->addChild(finish);
	menu->addChild(canel);
	menu->setColor(Color3B(100, 100, 100));
	menu->alignItemsHorizontallyWithPadding(15);

	this->addChild(bg);
	this->addChild(id,1);
	//this->addChild(idBG);
	//menu->setScale(1.5f);
	this->addChild(menu);
	//this->setTouchEnabled(true);
	return true;
}
void LoginScene::startCallback(Ref* ref) {
	MenuItemLabel* item = (MenuItemLabel*)ref;
	std::string name = item->getString();
	if (name == "Finish") {
		local_username = id->getString();
		if (local_username == "") {
			return;
		}
		auto scene = GameMode::createScene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
	}
	else if (name == "Cancel") {
		Director::getInstance()->getRunningScene()->getChildByTag(1)->removeChildByTag(1);
	}
}
void LoginScene::onEnter() {
	Layer::onEnter();
	auto callback = [](Touch *, Event *)
   {
	   return true;
   };
	auto listener= EventListenerTouchOneByOne::create(); 
	listener->onTouchBegan= callback;
	listener->setSwallowTouches(true); 
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);
}


