#include"IPScene.h"
#include"Settings.h"
#include"SimpleAudioEngine.h"
#include"RoomScene.h"
USING_NS_CC;

bool IPScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;

	bg = Sprite::create("UI/IP.png");
	bg->setScale(0.55);
	bg->setPosition(center_x, center_y);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	
	ip = ui::TextField::create(RoomScene::getIp(), "fonts/arial.ttf", 25);
	ip->setString(RoomScene::getIp());
	ip->setPosition(Vec2(center_x, center_y*1.1));
	ip->setColor(Color3B(150, 150, 150));
	ip->setTag(1);
	ip->setCursorEnabled(true);
	ip->setCursorChar('|');

	//idBG->setPosition(center_x, center_y);

	auto finish = MenuItemLabel::create(Label::create("Finish", "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(IPScene::startCallback, this));
	auto canel = MenuItemLabel::create(Label::create("Cancel", "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(IPScene::startCallback, this));
	auto menu = Menu::create();
	menu->setPosition(center_x, center_y * 2 / 3);
	menu->addChild(finish);
	menu->addChild(canel);
	menu->setColor(Color3B(100, 100, 100));
	menu->alignItemsHorizontallyWithPadding(15);

	this->addChild(bg);
	this->addChild(ip,1);
	//this->addChild(idBG);
	//menu->setScale(1.5f);
	this->addChild(menu);
	//this->setTouchEnabled(true);
	return true;
}


void IPScene::onEnter() {
	Layer::onEnter();
	auto callback = [](Touch *, Event *)
	{
		return true;
	};
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	
	this->runAction(popupLayer);
}
void IPScene::startCallback(Ref* ref) {
	MenuItemLabel* item = (MenuItemLabel*)ref;
	std::string name = item->getString();
	if (name == "Finish") {
		ser_ip = ip->getString();
		if (ser_ip == "") {
			return;
		}
		Director::getInstance()->getRunningScene()->removeChildByTag(1);
		Director::getInstance()->getRunningScene()->setColor(Color3B::WHITE);
		Director::getInstance()->getRunningScene()->removeChildByName("solid");
	}
	else if (name == "Cancel") {
		Director::getInstance()->getRunningScene()->removeChildByTag(1);
		Director::getInstance()->getRunningScene()->removeChildByName("solid");
	}
}