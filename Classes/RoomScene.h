#pragma once
#ifndef _ROOM_SCENE_H_
#define _ROOM_SCENE_H_

#include "cocos2d.h"

class RoomScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(RoomScene);
	void addPlayer();

	char * getIp();
	bool connectService();
private:
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	cocos2d::Sprite* player3;
	cocos2d::Sprite* player4;
	bool isReady;
	int players;// num of players
	
};
#endif // !_ROOM_SCENE_H_
