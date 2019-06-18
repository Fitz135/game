#pragma once
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"
#include"Player.h"
#include"Entity.h"
#include"ODSocket.h"

#include"RoomScene.h"


#define NewPlayer '1' 
#define KeyPress '2'
#define KeyRelease '3'
#define Ready '4'
#define GameStart '9'
#define Dialog '5'
#define MousePress '6'
#define MouseRelease '7'
#define DisConnect '8'

		extern int local_Id ;
		extern std::string local_username ;
		extern int Scale ;//Í¼Æ¬±ÈÀý

		extern std::vector<Entity*> playerList;
		extern std::vector<Vec2> posList;
		const int MSGSIZE = 32;
		extern int players;// num of players
		extern ODSocket* client;
		extern bool endThread;
		void getMsg(ODSocket* m_client);
		void updateDialog(char* buffer);
		void addPlayer(char* buffer);
		void gamestartCallback();
		void updatePlayer(char* buffer);
		void addMsg(std::string temp);

		namespace settings
		{
			constexpr char *weapon_paths[] = {
					"Bow.png",
					"Sword.png",
					"BubbleGun.png",
					"Boomerang.png",
					"Lance.png",
					"Shield.png"
			};
		}

		extern RoomScene* roomscene;
		extern bool isNewPlayer;
		extern bool isNewMsg;
		extern std::string newMsg;

		extern bool isWin;
		extern bool gameMode;
		extern std::string ser_ip;
#endif