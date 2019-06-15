#include "Settings.h"
#include"RoomScene.h"
#include"GameScene.h"
#include"OPOperator.h"
int local_Id=0;
std::string local_username="";
int Scale =1;
std::vector<Entity*> playerList;
std::vector<Vec2> posList;
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
		case NewPlayer:addPlayer(buffer); break;
		case GameStart: gamestartCallback();
					   log("gamestart\n"); break;
		case KeyPress:;
		case KeyRelease:;
		case MousePress:;
		case MouseRelease:updatePlayer(buffer); break;
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
	auto layer = dynamic_cast<RoomScene*>(Director::getInstance()->getRunningScene()->getChildByName("layer"));
	//layer->addMsg(temp);//readyCallback(NULL);
	
}
void addPlayer(char* buffer) {
	int start_pos = 2;
	char name[MSGSIZE];
	int id;

	for (int j = start_pos; j < MSGSIZE; j++) {
		if (buffer[j] == '$') {
			start_pos = j;
			break;
		}
		//name[j-start_pos] = buffer[j];
	}
	strncpy(name, &buffer[2], start_pos - 2);
	name[start_pos - 2] = '\0';
	id = static_cast<int>(buffer[start_pos+1]) - 48;
	//start_pos += 2;
	auto newPlayer = Player::create(std::string(name), id);
	auto entity = Entity::create(std::string(name), id);
	playerList.push_back(entity);
	players++;

	auto w = Director::getInstance()->getWinSize().width;
	auto h = Director::getInstance()->getWinSize().height;
	//auto label = Label::create(name, "fonts/arial.ttf", 15);
	//label->setPosition(0, 40);
	//label->setColor(Color3B::BLACK);
	newPlayer->setPosition(w*(2 + players) / 7, h * 3 / 4);
	//newPlayer->addChild(label);
	Director::getInstance()->getRunningScene()->addChild(newPlayer);
}
void gamestartCallback() {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}
void updatePlayer(char* buffer) {
	if (buffer[0] == KeyPress || buffer[0] == KeyRelease) {
		int id = static_cast<int>(buffer[2]) - 48;
		if (id != local_Id)
			dynamic_cast<OPOperator*>(
				dynamic_cast<Player*>(
					GameScene::getCurrentMap()->getChildByTag(id))->getChildByName("op"))->KeyStart(buffer);
	}
	else if (buffer[0] == MousePress || buffer[0] == MouseRelease) {
		int id = static_cast<int>(buffer[2]) - 48;
		if (id != local_Id)
			dynamic_cast<OPOperator*>(
				dynamic_cast<Player*>(
					GameScene::getCurrentMap()->getChildByTag(id))->getChildByName("op"))->MouseStart(buffer);
	}
}