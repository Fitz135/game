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
		}
	}
}