#pragma once
#include "cocos2d.h"
#include"Player.h"
#include"ODSocket.h"
#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'

		extern int local_Id ;
		extern std::string local_username ;
		extern int Scale ;//Í¼Æ¬±ÈÀý

		extern std::vector<Player*> playerList;

		const int MSGSIZE = 32;
		extern int players;// num of players
		extern ODSocket* client;
		namespace settings
		{
			constexpr char *weapon_paths[] = {
					"Bow.png",
					"Sword.png",
					"BubbleGun.png",
					"Boomerang.png"
			};
		}