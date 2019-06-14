#include "RoomScene.h"
#include"GameScene.h"
#include"ui/CocosGUI.h"
#include"Settings.h"
#include"Entity.h"
#include"thread"
USING_NS_CC;

static bool endThread;

Scene* RoomScene::createScene() {
	auto scene = Scene::create();
	auto layer = RoomScene::create();
	scene->addChild(layer);
	return scene;
}


bool RoomScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto center_x = Director::getInstance()->getWinSize().width / 2;
	auto center_y = Director::getInstance()->getWinSize().height / 2;
	
	auto imgBG = Sprite::create("UI/RoomBG.png");
	imgBG->setPosition(center_x, center_y);
	this->addChild(imgBG);
	char buffer[MSGSIZE];
	connectService(buffer);
	initPlayer(buffer);

	auto readyItem = MenuItemLabel::create(Label::create("Ready", "arial.ttf", 30), CC_CALLBACK_1(RoomScene::readyCallback, this));
	auto menu = Menu::create();
	menu->addChild(readyItem);
	readyItem->setPosition(-center_x*4/5, -center_y*4/5);
	this->addChild(menu,1);
	
	return true;
}


void RoomScene::connectService(char* buffer){

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char * ip = getIp();

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
		char* name;
		int id;
		for (int j = start_pos; j < MSGSIZE; j++) {
			if (buffer[j] = '$') {
				start_pos = j +1;
				break;
			}
			name[j-start_pos] = buffer[j];
		}
		id = static_cast<int>(buffer[start_pos])-48;
		start_pos+=2;
		//std::string(name);
		playerList.push_back(Entity::create(std::string(name), id));
	}

	//////set position/////
	auto w = Director::getInstance()->getWinSize().width;
	auto h = Director::getInstance()->getWinSize().height;
	for (int i = 0; i < players; i++) {
		auto label = Label::create(playerList[i]->getName(), "arial.ttf", 15);
		auto player = Player::create(playerList[i]->getName(), playerList[i]->getId());
		if (i == 0) {
			player->setPosition(w/4, h*4/7);
			player->setScale(1.5f);
		}
		else {
			player->setPosition(w*(3+i)/7, h*3/4);
		}
		label->setPosition(0, 40);
		label->setColor(Color3B::BLACK);
		player->addChild(label);
		this->addChild(player);
	}
	
	return true;
}
void RoomScene::readyCallback(Ref* ref) {
	/*
	
	auto scene =GameScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
	
	*/
	MenuItemLabel* item = (MenuItemLabel*)ref;
	char buffer[MSGSIZE];
	if (item->getString() == "Ready") {
		item->setString("Unready");
		sprintf(buffer, "%c$%d$r", Ready,playerList[0]->getId());
		client->Send(buffer, 5);
	}
	else {
		item->setString("Ready");
		sprintf(buffer, "%c$%d$u", Ready,playerList[0]->getId());
		client->Send(buffer, 5);
	}
}
void RoomScene::gamestartCallback() {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}
void RoomScene::onEnter(){
	Layer::onEnter();
	endThread = 0;
	std::thread t(getMsg,client);
	t.detach();
}
void RoomScene::onExit() {
	Layer::onExit();
	endThread = 1;
}
void RoomScene::addPlayer(char* buffer) {
	int start_pos = 2;
	char* name;
	int id;

	for (int j = start_pos; j < MSGSIZE; j++) {
		if (buffer[j] = '$') {
			start_pos = j + 1;
			name[j] = '\0';
			break;
		}
		name[j - start_pos] = buffer[j];
	}
	id = static_cast<int>(buffer[start_pos]) - 48;
	start_pos += 2;
	auto newPlayer = Player::create(std::string(name), id);
	auto entity= Entity::create(std::string(name), id);
	playerList.push_back(entity);
	players++;

	auto w = Director::getInstance()->getWinSize().width;
	auto h = Director::getInstance()->getWinSize().height;
	auto label = Label::create(newPlayer->getName(), "arial.ttf", 15);
	label->setPosition(0, 40);
	label->setColor(Color3B::BLACK);
	newPlayer->setPosition(w*(2+players) / 7, h * 3 / 4);
	newPlayer->addChild(label);
	Director::getInstance()->getRunningScene()->addChild(newPlayer);
}