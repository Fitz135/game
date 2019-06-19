#include"ResultScene.h"
#include"Settings.h"
#include"SimpleAudioEngine.h"
USING_NS_CC;

bool ResultScene::init() {
	if (!Layer::init()) {
		return false;
	}
	
	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;

	if (isWin) {
		bg = Sprite::create("UI/Victory.png");
		bg->setPosition(center_x, center_y*1.2);
	}
	else {
		bg = Sprite::create("UI/Lose.png");
		bg->setPosition(center_x, center_y*1.35);
		bg->setScale(1.3);
		bg->setScaleY(1.4);
	}
	if (bg == nullptr) { log("fail"); return false; }
	//bg->setPosition(center_x, center_y*1.2);
	this->addChild(bg);

	char* tmp;
	if (!gameMode) {
		tmp = "Return to room";
	}
	else {
		tmp = "Mode Select";
	}
	auto returnroomItem = MenuItemLabel::create(
		Label::create(tmp, "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(ResultScene::reroomCallback, this));
	auto exitItem= MenuItemLabel::create(
		Label::create("Exit", "fonts/Cordelia.ttf", 45), CC_CALLBACK_1(ResultScene::exitCallback, this));

	auto menu = Menu::create();
	menu->addChild(returnroomItem);
	menu->addChild(exitItem);
	menu->alignItemsVerticallyWithPadding(30);
	menu->setPosition(center_x, center_y*0.7);
	this->addChild(menu);


	return true;
}
void ResultScene::reroomCallback(Ref* ref) {

		Director::getInstance()->popScene();
}
void ResultScene::exitCallback(Ref* ref) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	if (client != nullptr) {
		char buffer[MSGSIZE];
		sprintf(buffer, "%c$%d", DisConnect, local_Id);
		client->Send(buffer, MSGSIZE);
		client->Close();
	}
	Director::getInstance()->end();
}

void ResultScene::onEnter() {
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
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (isWin) {
		audio->playEffect("audio/victory.wav", false); }
	else{
		audio->playEffect("audio/death.wav", false);
	}
	this->runAction(popupLayer);
}