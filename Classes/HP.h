#pragma once
#ifndef _HP_H_
#define _HP_H_
#include"cocos2d.h"
#include"ui/CocosGUI.h"
class Hp :public cocos2d::Layer {
public:
	CREATE_FUNC(Hp);
	virtual bool init();
	void setHp(float );
private:
	cocos2d::ui::LoadingBar* HpBar;
	cocos2d::Sprite* loadingbar;
	
};
#endif // !_HP_H_
