#pragma once
#ifndef _GAMEMODE_SCENE_H_
#define __GAMEMODE_SCENE_H_

#include<cocos2d.h>

class GameMode :public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void gamestartCallback(Ref* ref);
	void menubackCallback(Ref* ref);

	CREATE_FUNC(GameMode);
};
#endif // !_GAMEMODE_SCENE_H_
