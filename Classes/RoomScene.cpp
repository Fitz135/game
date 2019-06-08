#include "RoomScene.h"
#include"ui/CocosGUI.h"
#include"ODSocket.h"
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

	if (connectService()) {
		
	}
	
}
bool RoomScene::connectService() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char * ip = getIp();

	auto client = new ODSocket();
	client->Init();
	bool res = client->Create(AF_INET, SOCK_STREAM, 0);
	log("Create:");
	res = client->Connect(ip, 2111);//
	log("Connect:");
	if (res) {
		char buffer[30];
		client->Recv(buffer, 1);
		players = static_cast<int>(buffer[0])-48;
		sprintf(buffer, "%d", players);
		client->Send(buffer, 1);
		client->Send("q", 1);
		client->Close();
	}
	return true;
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