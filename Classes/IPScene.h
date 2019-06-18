#pragma once
#ifndef _IP_SCENE_H_
#define _IP_SCENE_H_

#include <cocos2d.h>
#include<ui/CocosGUI.h>

class IPScene :public cocos2d::Layer {
public:
	virtual bool init();
	CREATE_FUNC(IPScene);
	virtual void onEnter();
	void startCallback(cocos2d::Ref* ref);
	//bool onTouchBegan(Touch *touch, Event *unused_event);

private:
	cocos2d::Sprite* bg;
	cocos2d::ui::TextField * ip;
};
#endif // !_IP_SCENE_H_