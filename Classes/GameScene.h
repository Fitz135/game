#pragma once
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include"cocos2d.h"

class GameScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	static  GameScene* getCurrentMap();

};
#endif // !_GAME_SCENE_H_
