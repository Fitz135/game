#pragma once
#include "cocos2d.h"
#include"Player.h"
#include"ODSocket.h"
#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'
#define Ready '4'

		extern int local_Id ;
		extern std::string local_username ;
		extern int Scale ;//ͼƬ����

		extern std::vector<Player*> playerList;

		const int MSGSIZE = 32;
		extern int players;// num of players
		extern ODSocket* client;