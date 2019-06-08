#pragma once
#ifndef _LOGIN_SCENE_H_
#define _LOGIN_SCENE_H_

#include <cocos2d.h>

class LoginScene :public cocos2d::Layer {
public:
	virtual bool init();
	CREATE_FUNC(LoginScene);
	virtual void onEnter();
	void startCallback(Ref* ref);
	//bool onTouchBegan(Touch *touch, Event *unused_event);
private:
	cocos2d::Sprite* bg;
};
#endif // !_LOGIN_SCENE_H_
