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
	char* connectService();
	bool initPlayer(char *);
private:
	bool isReady[4];
	int players;// num of players
	
};
#endif // !_ROOM_SCENE_H_
