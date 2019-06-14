#include "Settings.h"
#include"RoomScene.h"
#include"GameScene.h"
int local_Id=0;
std::string local_username="";
int Scale =1;
std::vector<Player*> playerList;
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
		m_client->Recv(buffer, MSGSIZE);
		log(buffer);
		switch (buffer[0]) {
		case NewPlayer:RoomScene::addPlayer(buffer); break;
		case GameStart: RoomScene::gamestartCallback();
					   log("gamestart\n"); break;
		case KeyPress:;
		case KeyRelease:;//dynamic_cast<OPOperator*>(player->getChildByName("op"))->KeyStart(buffer); break;
		}
	}
}