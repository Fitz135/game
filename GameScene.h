#pragma once
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include"cocos2d.h"
#include"Player.h"
#include"Robot.h"
#include<vector>

class GameScene :public cocos2d::Scene {
public:
	
	GameScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
private:
	Player* player;
	std::vector<Robot> roboyVec;

};
#endif // !_GAME_SCENE_H_
