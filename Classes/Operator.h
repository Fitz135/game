#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include "cocos2d.h"
#include"Player.h"
using namespace cocos2d;

class Operator : public Layer
{
public:
	CREATE_FUNC(Operator);
private:
	virtual bool init();

	void KeyStart();
	void TouchStart();
	void PassOperatorInfo(float dt);

	bool onTouchBegan(Touch * ptouch, Event * pevent);
	void onTouchMoved(Touch * ptouch, Event * pevent);
	void onThouchEnded(Touch * ptouch, Event * pevent);
	void OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	int PressNum=0;
	bool MouseDown=0;
	Point MousePosition;

	void MoveUP(float dt);
	void MoveDOWN(float dt);
	void MoveLEFT(float dt);
	void MoveRIGHT(float dt);
	
	const SEL_SCHEDULE move[4] = {
	schedule_selector(Operator::MoveLEFT),
	schedule_selector(Operator::MoveRIGHT),
	schedule_selector(Operator::MoveUP),
	schedule_selector(Operator::MoveDOWN) };
};
#endif


