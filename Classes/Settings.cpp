#include "Settings.h"
#include"RoomScene.h"
#include"GameScene.h"
int local_Id=0;
std::string local_username="";
int Scale =1;
std::vector<Entity*> playerList;
int players=0;

ODSocket* client;
bool endThread = 0;
void getMsg(ODSocket* m_client) {
	char buffer[MSGSIZE];
	while (!endThread) {
		if (client == NULL) {
			log("null client");
			break;
		}
		memset(buffer, 0, sizeof(buffer));
		m_client->Recv(buffer, MSGSIZE);
		log(buffer);
		switch (buffer[0]) {
		case NewPlayer:RoomScene::addPlayer(buffer); break;
		case GameStart: RoomScene::gamestartCallback();
					   log("gamestart\n"); break;
		case KeyPress:;
		case KeyRelease:GameScene::updatePlayer(buffer); break;
		case Dialog:updateDialog(buffer); break;
		}
	}
}
void updateDialog(char* buffer) {
	int id = static_cast<int>(buffer[2]) - 48;
	std::string name;
	for (int i = 0; i < players; i++) {
		if (playerList[i]->_id == id) {
			name = std::string(playerList[i]->_name);
			break;
		}
	}
	char msg[MSGSIZE];
	strcpy(msg, &buffer[4]);
	char finaltext[sizeof(name) + sizeof(msg) + 1];
	sprintf(finaltext, "%s:%s", name.c_str(), msg);
	//log(finaltext);
	//int len = name.length() + strlen(msg) + 1;
	std::string temp(finaltext);
	//int t = temp.length();
	auto hello = ui::Text::create(temp, "fonts/arial.ttf", 15);
	hello->ignoreContentAdaptWithSize(false);
	hello->setColor(cocos2d::Color3B::BLUE);
	auto layer = dynamic_cast<RoomScene*>(Director::getInstance()->getRunningScene()->getChildByName("layer"));
	auto dial=dynamic_cast<ui::ListView*>(layer->getChildByName("ListView"));
	dial->addChild(hello, 1);
	if (dial->getChildrenCount() > 6)dial->removeItem(0);
	dial->forceDoLayout();
	dial->jumpToBottom();
}