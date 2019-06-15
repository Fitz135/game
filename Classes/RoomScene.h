#pragma once
#ifndef _ROOM_SCENE_H_
#define _ROOM_SCENE_H_

#include "cocos2d.h"
#include"ui/CocosGUI.h"

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
	void sendCallback(Ref*);
	static void gamestartCallback();
	static void updateDialog(char*);
	//void getMsg(ODSocket*);
	virtual void  onEnter();
	virtual void  onExit();

	void initChat();
	
private:
	bool isReady[4];
	cocos2d::ui::ListView* dialog;
	cocos2d::ui::TextField* text;
	
	
	
};
#endif // !_ROOM_SCENE_H_
