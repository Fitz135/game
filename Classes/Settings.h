#pragma once
#include "cocos2d.h"
#include"Player.h"
#include"Entity.h"
#include"ODSocket.h"
#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'
#define Ready '4'
#define GameStart '9'
#define Dialog '5'
		
		extern int local_Id ;
		extern std::string local_username ;
		extern int Scale ;//Í¼Æ¬±ÈÀý

		extern std::vector<Entity*> playerList;

		const int MSGSIZE = 32;
		extern int players;// num of players
		extern ODSocket* client;
		extern bool endThread;
		void getMsg(ODSocket* m_client);
		void updateDialog(char* buffer);
		namespace settings
		{
			constexpr char *weapon_paths[] = {
					"Bow.png",
					"Sword.png",
					"BubbleGun.png",
					"Boomerang.png"
			};
		}