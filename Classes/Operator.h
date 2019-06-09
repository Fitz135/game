#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include "cocos2d.h"
#include"Player.h"
using namespace cocos2d;

class Operator : public Layer
{
public:
	void KeyStart();
	void MouseStart();

	void PassOperatorInfo(float dt);

	bool onMouseBegan(Touch * ptouch, Event * pevent);
	void onMouseMoved(Touch * ptouch, Event * pevent);
	void onMouseEnded(Touch * ptouch, Event * pevent);

	void OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	int PressNum;
	bool MouseDown;
	Point MousePosition;

	void MoveUP(float dt);
	void MoveDOWN(float dt);
	void MoveLEFT(float dt);
	void MoveRIGHT(float dt);
	CREATE_FUNC(Operator);
	virtual bool init();
};
#endif


