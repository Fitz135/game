#include "Settings.h"
#include"RoomScene.h"
#include"GameScene.h"
#include"OPOperator.h"
int local_Id=0;
std::string local_username="";
int Scale =1;
std::vector<Entity*> playerList;
std::vector<Vec2> posList;
std::list<std::string> cmdList;
Vec3 propPos;
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
		case GameStart: gamestartCallback();break;
		case KeyPress:;
		case KeyRelease:;
		case MousePress:;
		case MouseRelease:updatePlayer(buffer); break;
		case Dialog:updateDialog(buffer); break;
		case Prop:addProp(buffer); break;
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
	
	
	newMsg= std::string(finaltext);
	isNewMsg = true;
	
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
	}
	strncpy(name, &buffer[2], start_pos - 2);
	name[start_pos - 2] = '\0';
	id = static_cast<int>(buffer[start_pos+1]) - 48;
	
	auto entity = Entity::create(std::string(name), id);
	playerList.push_back(entity);
	players++;
	isNewPlayer = true;
}
void gamestartCallback() {
	auto scene = GameScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
}
void updatePlayer(char* buffer) {

	cmdList.push_back(std::string(buffer));

}
void addProp(char* buffer) {
	isNewProp = true;
	int x = 0, y = 0;
	char c_x[5];
	char c_y[5];
	int start_pos = 2;
	int tps = 2;
	for (int j = start_pos; j < MSGSIZE; j++) {
		if (buffer[j] == '$') {
			start_pos = j;
			break;
		}
	}
	strncpy(c_x, &buffer[tps], start_pos - tps);
	c_x[start_pos - tps] = '\0';
	for (int i = 0; i < start_pos - tps; i++) {
		int q = 1;
		for (int k = 1; k < start_pos - tps - i; k++) {
			q *= 10;
		}
		x += (static_cast<int>(buffer[tps + i]) - 48)*q;
	}
	tps = start_pos + 1;
	for (int j = start_pos + 1; j < MSGSIZE; j++) {
		if (buffer[j] == '$') {
			start_pos = j;
			break;
		}
	}
	strncpy(c_y, &buffer[tps], start_pos - tps);
	c_y[start_pos - tps] = '\0';
	for (int i = 0; i < start_pos - tps; i++) {
		int q = 1;
		for (int k = 1; k < start_pos - tps - i; k++) {
			q *= 10;
		}
		y += (static_cast<int>(buffer[tps + i]) - 48)*q;
	}
	int type = static_cast<int>(buffer[start_pos + 1]) - 48;
	propPos = cocos2d::Vec3(x, y, type);
}
RoomScene *roomscene;

bool isNewPlayer = false;
bool isNewMsg = false;
bool isNewProp = false;
std::string newMsg;

bool isWin = true; 
bool gameMode;
std::string ser_ip="NULL";
