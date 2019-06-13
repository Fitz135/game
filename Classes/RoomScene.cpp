#include "RoomScene.h"
#include"ui/CocosGUI.h"
#include"ODSocket.h"
#include"Settings.h"
USING_NS_CC;

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

	return initPlayer(connectService());
}


char* RoomScene::connectService() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char * ip = getIp();

	auto client = new ODSocket();
	client->Init();
	bool res = client->Create(AF_INET, SOCK_STREAM, 0);
	log("Create:");
	res = client->Connect(ip, 2111);//
	log("Connect:");
	char buffer[MSGSIZE];
	if (res) {
		
		client->Recv(buffer, MSGSIZE);
		client->Send(local_username.c_str(), MSGSIZE);

		players = static_cast<int>(buffer[0])-48;
		local_Id = players;
	}
	return buffer;
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

	auto player=Player::create(local_username, local_Id);
	playerList.push_back(player);
	int start_pos = 2;
	char* name;
	int id;
	for (int i = 0; i < players-1; i++) {
		for (int j = start_pos; j < MSGSIZE; j++) {
			if (buffer[j] = '\t') {
				start_pos = j +1;
				break;
			}
			name[j-start_pos] = buffer[j];
		}
		id = static_cast<int>(buffer[start_pos])-48;
		start_pos+=2;
		std::string(name);
		playerList.push_back(Player::create(std::string(name), id));
	}
	auto w = Director::getInstance()->getWinSize().width / (players+1);
	auto h = Director::getInstance()->getWinSize().height/ (players + 1);
	for (int i = 0; i < players; i++) {
		auto label = Label::create(playerList[i]->getName(), "arial.ttf", 15);
		playerList[i]->setPosition(w*(i + 1), h*(i + 1));
		label->setPosition(0, 40);
		playerList[i]->addChild(label);
		this->addChild(playerList[i]);
	}
	return true;
}