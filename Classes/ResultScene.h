#pragma once
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include "cocos2d.h"

class ResultScene :public cocos2d::Layer {
public:
	virtual bool init();
	CREATE_FUNC(ResultScene);
	virtual void onEnter();
	void reroomCallback(cocos2d::Ref* ref);
	void exitCallback(cocos2d::Ref* ref);

	
private:
		cocos2d::Sprite* bg;
		//cocos2d::ui::TextField * id;
};
#endif // !_RESULT_SCENE_H_
