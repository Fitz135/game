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

	bg = Sprite::create("UI/PlayerBox.png");
	bg->setPosition(center_x, center_y);

	id = ui::TextField::create("input your ID ", "arial.ttf", 15);
	auto idBG = Sprite::create("UI/TextFieldBG.png");
	id->setPosition(Vec2(center_x, center_y));
	id->setTag(1);
	idBG->setPosition(center_x, center_y);

	auto finish = MenuItemLabel::create(Label::create("Finish", "arial.ttf", 20), CC_CALLBACK_1(LoginScene::startCallback, this));
	auto canel = MenuItemLabel::create(Label::create("Cancel", "arial.ttf", 20), CC_CALLBACK_1(LoginScene::startCallback, this));
	auto menu = Menu::create();
	menu->setPosition(center_x, center_y * 2 / 3);
	menu->addChild(finish);
	menu->addChild(canel);
	menu->alignItemsHorizontallyWithPadding(15);

	this->addChild(bg);
	this->addChild(id,1);
	this->addChild(idBG);
	this->addChild(menu);
	this->setTouchEnabled(true);
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
		this->setVisible(false);
	}
}
void LoginScene::onEnter() {
	Layer::onEnter();
	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);
}
