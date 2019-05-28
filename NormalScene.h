#pragma once
#ifndef _NORMAL_SCENE_H_
#define _NORMAL_SCENE_H_

#include<cocos2d.h>

class Normal :public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menubackCallback(Ref* ref);
	CREATE_FUNC(Normal);
};
#endif // !_NORMAL_SCENE_H_
