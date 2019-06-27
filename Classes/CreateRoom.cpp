#include "CreateRoom.h"
#include"ui/CocosGUI.h"
USING_NS_CC;
/*CreateRoom* CreateRoom::create() {
	auto room = new (std::nothrow) CreateRoom();
	auto imgBG = Sprite::create("UI/CreateRoomBG.png");
	room->addChild(imgBG);
	if (room&&room->init())
	{
		room->autorelease();
		return room;
	}
}*/
bool CreateRoom::init() {
	if (!Layer::init()) {
		return false;
	}
	
	return true;
}

void CreateRoom::onEnter() {
	Layer::onEnter();
	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;

	auto title = Label::create("Create a room", "arial.ttf", 20);
	auto idLabel = Label::create("Your Id:", "arial.ttf", 20);
	auto numLabel = Label::create("num of players:", "arial.ttf", 20);
	auto inputBG = Sprite::create("UI/TextFieldBG.png");
	auto inputBG2 = Sprite::create("UI/TextFieldBG.png");
	auto id = ui::TextField::create("input your ID (12c most)", "arial.ttf", 15);
	auto num = ui::TextField::create("input players' num (4 most)", "arial.ttf", 15);
	auto imgBG = Sprite::create("UI/CreateRoomBG.png");
	imgBG->setPosition(center_x, center_y);
	float scale = 0.5;
	inputBG->setScale(scale);
	inputBG2->setScale(scale);
	auto pedding= Director::getInstance()->getWinSize().width/20;
	idLabel->setPosition(center_x - inputBG->getContentSize().width / 2*scale-10, center_y + inputBG->getContentSize().height / 2 * scale + 5);
	id->setPosition(Vec2(center_x + inputBG->getContentSize().width / 2 * scale - 10, center_y + inputBG->getContentSize().height / 2 * scale + 5));
	inputBG->setPosition(Vec2(center_x + inputBG->getContentSize().width / 2 * scale - 10, center_y + inputBG->getContentSize().height / 2 * scale + 5));
	numLabel->setPosition(center_x - inputBG->getContentSize().width / 2 - 10, center_y - inputBG->getContentSize().height / 2 - 5);
	num->setPosition(Vec2(center_x + inputBG->getContentSize().width / 2 - 10, center_y - inputBG->getContentSize().height / 2 + -5));
	inputBG2->setPosition(Vec2(center_x + inputBG->getContentSize().width / 2 - 10, center_y - inputBG->getContentSize().height / 2 - 5));
	title->setPosition(center_x, idLabel->getPosition().y + 20);
	
	this->addChild(title);
	this->addChild(imgBG,-2);
	this->addChild(id);
	this->addChild(num);
	this->addChild(idLabel);
	this->addChild(numLabel);
	this->addChild(inputBG,-1);
	this->addChild(inputBG2,-1);

	auto createLabel = Label::create("Create", "arial.ttf", 20);
	auto createItem = MenuItemLabel::create(createLabel);
	auto cancelLabel = Label::create("Cancel", "arial.ttf", 20);
	auto cancelItem = MenuItemLabel::create(cancelLabel);
	auto menu = Menu::create();
	menu->addChild(createItem);
	menu->addChild(cancelItem);
	menu->alignItemsHorizontallyWithPadding(5);
	menu->setPosition(center_x, num->getPosition().y - 10);
	this->addChild(menu);
	

	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);
}