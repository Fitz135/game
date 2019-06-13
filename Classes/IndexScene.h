#pragma once
#ifndef _INDEX_SCENE_H_
#define _INDEX_SCENE_H_

#include<cocos2d.h>

class Index :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void startCallback(Ref* ref);
	void exitCallback(Ref* ref);

	CREATE_FUNC(Index); //宏展开为 create()函数；

};
#endif // _INDEX_SCENE_H_
