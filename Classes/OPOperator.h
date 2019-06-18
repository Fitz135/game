#pragma once
#ifndef __OPOPERATOR_H__
#define __OPOPERATOR_H__

#include "cocos2d.h"

using namespace cocos2d;

class OPOperator : public Layer
{
public:
	void KeyStart(const char*);
	void MouseStart(const char*);

	//void AIMove(char * buffer);

	void PassOperatorInfo(float dt);

	bool onMouseBegan(Touch * ptouch, Event * pevent);
	void onMouseMoved(Touch * ptouch, Event * pevent);
	void onMouseEnded(Touch * ptouch, Event * pevent);
	bool Isschedule[4] = { 0,0,0,0, };


	int PressNum;
	bool MouseDown;
	Point MousePosition;

	void MoveUP(float dt);
	void MoveDOWN(float dt);
	void MoveLEFT(float dt);
	void MoveRIGHT(float dt);
	CREATE_FUNC(OPOperator);
	virtual bool init();
};
#endif


