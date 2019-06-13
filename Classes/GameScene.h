#pragma once
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include"cocos2d.h"
#include "Player.h"

class GameScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	static  GameScene* getCurrentMap();

	virtual void onEnter();
	virtual void onExit();
	
	
	
};
#endif // !_GAME_SCENE_H_
