#pragma once
#ifndef _INDEX_SCENE_H_
#define _INDEX_SCENE_H_

#include<cocos2d.h>
#include"ui/CocosGUI.h"
class Index :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void startCallback(Ref* ref);
	void exitCallback(Ref* ref);
	virtual void onEnter();
	CREATE_FUNC(Index); //��չ��Ϊ create()������

	
};
#endif // _INDEX_SCENE_H_
