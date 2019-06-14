#pragma once
#ifndef _ROOM_SCENE_H_
#define _ROOM_SCENE_H_

#include "cocos2d.h"


class RoomScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(RoomScene);
	static void addPlayer(char*);

	char * getIp();
	void connectService(char*);
	bool initPlayer(char *);

	void readyCallback(Ref*);
	static void gamestartCallback();
	//void getMsg(ODSocket*);
	virtual void  onEnter();
	virtual void  onExit();

	
private:
	bool isReady[4];

	
	
	
};
#endif // !_ROOM_SCENE_H_
