#include "RoomScene.h"
#include"GameScene.h"

#include"Settings.h"
#include"Entity.h"
#include"thread"
USING_NS_CC;

static bool endThread;

Scene* RoomScene::createScene() {
	auto scene = RoomScene::create();
	/*auto layer = RoomScene::create();
	layer->setName("layer");
	scene->addChild(layer);*/
	return scene;
}


bool RoomScene::init() {
	if (!Scene::init()) {
		return false;
	}

	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;
	
	auto imgBG = Sprite::create("UI/RoomBG.png");
	imgBG->setPosition(center_x, center_y);
	imgBG->setScaleX(2.2);
	imgBG->setScaleY(2.85);
	this->addChild(imgBG,-1);
	char buffer[MSGSIZE];
	connectService(buffer);
	initPlayer(buffer);

	auto menu = Menu::create();
	auto readyItem = MenuItemLabel::create(Label::create("Ready", "fonts/Cordelia.ttf", 47), CC_CALLBACK_1(RoomScene::readyCallback, this));
	menu->addChild(readyItem);
	readyItem->setPosition(-center_x*0.58, -center_y*0.55);
	auto backItem = MenuItemLabel::create(Label::create("Back", "fonts/Cordelia.ttf", 47), CC_CALLBACK_1(RoomScene::backCallback, this));
	menu->addChild(backItem);
	backItem->setPosition(-center_x * 0.58, -center_y * 0.83);
	auto sendItem= MenuItemLabel::create(Label::create("Send", "fonts/Cordelia.ttf", 34), CC_CALLBACK_1(RoomScene::sendCallback, this));
	menu->addChild(sendItem);
	sendItem->setPosition(center_x * 0.83, -center_y * 0.85);
	menu->setColor(Color3B(143, 137, 137));
	this->addChild(menu,1);

	initChat();
	this->scheduleUpdate();
	return true;
}


void RoomScene::connectService(char* buffer){

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char * ip = "192.168.1.106";//getIp();

	client = new ODSocket();
	client->Init();
	bool res = client->Create(AF_INET, SOCK_STREAM, 0);
	log("Create:");
	res = client->Connect(ip, 2111);//
	log("Connect:");
	if (res) {
		
		client->Recv(buffer, MSGSIZE);
		client->Send(local_username.c_str(), MSGSIZE);

		players = static_cast<int>(buffer[0])-48;
		local_Id = players;
	}
}
char * RoomScene::getIp()
{
	PHOSTENT hostinfo;
	char name[255];
	char* ip; log("host");
	int ok = gethostname(name, sizeof(name));
	if (ok == 0)
	{

		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			return ip;
		}
	}
	return NULL;
}
bool RoomScene::initPlayer(char* buffer) {

	auto player=new Entity(local_username, local_Id);
	playerList.push_back(player);


	////init other player////////
	for (int i = 0; i < players-1; i++) {
		int start_pos = 2;
		char name[12];
		int id;
		for (int j = start_pos; j < MSGSIZE; j++) {
			if (buffer[j]== '$') {
				start_pos = j ;
				break;
			}
			//name[j-start_pos] = buffer[j];
		}
		strncpy(name, &buffer[2], start_pos - 2);
		name[start_pos - 2] = '\0';
		id = static_cast<int>(buffer[start_pos+1])-48;
		start_pos+=2;
		//std::string(name);
		playerList.push_back(Entity::create(std::string(name), id));
	}

	//////set position/////
	auto w = Director::getInstance()->getWinSize().width;
	auto h = Director::getInstance()->getWinSize().height;
	for (int i = 0; i < players; i++) {
		//auto label = Label::create(playerList[i]->getName(), "arial.ttf", 15);
		auto player = Player::create(playerList[i]->getName(), playerList[i]->getId());
		if (i == 0) {
			player->setPosition(w*0.23, h*0.65);
			player->setScale(3.0f);
		}
		else {
			player->setPosition(w*(4.5+i*3.5)/17, h*3/4);
			player->setScale(2.0f);
		}
		//label->setPosition(0, 40);
		//label->setColor(Color3B::BLACK);
		//player->addChild(label);
		this->addChild(player);
	}
	
	return true;
}
void RoomScene::readyCallback(Ref* ref) {
	MenuItemLabel* item = (MenuItemLabel*)ref;
	char buffer[MSGSIZE];
	if (item->getString() == "Ready") {
		item->setString("UnReady");
		sprintf(buffer, "%c$%d$r", Ready,playerList[0]->_id);
		client->Send(buffer, MSGSIZE);
	}
	else if(item->getString() == "UnReady"){
		item->setString("Ready");
		sprintf(buffer, "%c$%d$u", Ready,playerList[0]->_id);
		client->Send(buffer, MSGSIZE);
	}
}
void RoomScene::backCallback(Ref* ref) {
	Director::getInstance()->popScene();
	client->Close();
}
void RoomScene::sendCallback(Ref* ref) {
	char buffer[MSGSIZE];
	auto msg=dynamic_cast<ui::TextField*>(this->getChildByName("TextField"))->getString().c_str();
	sprintf(buffer, "%c$%d$%s", Dialog, local_Id, msg);
	client->Send(buffer, MSGSIZE);
	//addMsg(dynamic_cast<ui::TextField*>(this->getChildByName("TextField"))->getString());
}

void RoomScene::onEnter(){
	Scene::onEnter();
	endThread = 0;
	std::thread t(getMsg,client);
	t.detach();
}
void RoomScene::onExit() {
	Scene::onExit();
	//endThread = 1;
}


void RoomScene::initChat() {
	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;

	text = ui::TextField::create("Enter the message here", "fonts/arial.ttf", 30);
	text->setPosition(Vec2(center_x*1.2 , center_y*0.185 ));
	text->setName("TextField");
	text->setCursorEnabled(true);
	text->setCursorChar('|');
	this->addChild(text);

	/*msglog = ui::Text::create("chat here", "fonts/arial.ttf", 40);
	msglog->setPosition(Vec2(center_x, center_y));

	this->addChild(msglog);*/

	
	
	dialog= ui::ListView::create();
	dialog->setDirection(ui::ScrollView::Direction::VERTICAL);
	dialog->setContentSize(Size(551, 260));//33个30大小的字符，6行
	dialog->setName("ListView");
	dialog->setPosition(Vec2(center_x*0.83, center_y*0.40));
	this->addChild(dialog,1);
	/*
	for (int i = 0; i < 10; i++) {
		auto hello = ui::Text::create("123456678912345678912345678", "fonts/arial.ttf",15);
		hello->ignoreContentAdaptWithSize(false);
		hello->setColor(cocos2d::Color3B::BLUE);
		//hello->setPosition(Vec2(center_x, -center_y / 2));
		dialog->addChild(hello,1);
	}*/
	//dialog->jumpToBottom();
	
	
}
void RoomScene::update(float dt) {
	if (isNewMsg) {
		auto hello = ui::Text::create(newMsg, "fonts/arial.ttf", 30);
		hello->ignoreContentAdaptWithSize(false);
		hello->setColor(cocos2d::Color3B::BLUE);
		dialog->addChild(hello, 1);
		isNewMsg = false;
		if (dialog->getChildrenCount() >= 30)dialog->removeItem(0);
		dialog->forceDoLayout();
		dialog->jumpToBottom();
		//dialog->scrollToBottom(0.5, false);
	}
	if (isNewPlayer) {
		auto name = std::string(playerList[players - 1]->_name);
		auto id = playerList[players - 1]->_id;
		auto newPlayer = Player::create(name, id);
		auto w = Director::getInstance()->getWinSize().width;
		auto h = Director::getInstance()->getWinSize().height;
		auto label = Label::create(name, "fonts/arial.ttf", 15);
		label->setPosition(0, 40);
		label->setColor(Color3B::BLACK);
		newPlayer->setPosition(w*(1 + players * 3.5) / 17, h * 3 / 4);
		newPlayer->addChild(label);
		this->addChild(newPlayer);
		isNewPlayer = false;
	}
}
