#pragma once
#ifndef _CREATE_ROOM_H_
#define _CREATE_ROOM_H_

#include <cocos2d.h>


class CreateRoom :public cocos2d::Layer {
public:
	//CreateRoom();
	//~CreateRoom();
	virtual bool init();
	CREATE_FUNC(CreateRoom);
	//static CreateRoom* create();
	virtual void onEnter();
	//virtual void onExit();
private:
	//Sprite* inputBG;
};
#endif // !_CREATE_ROOM_H_
