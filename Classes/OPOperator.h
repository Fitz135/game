#pragma once
#ifndef __OPOPERATOR_H__
#define __OPOPERATOR_H__

#include "cocos2d.h"

using namespace cocos2d;

class OPOperator : public Layer
{
public:
	CREATE_FUNC(OPOperator);

	void KeyStart(const char*);
	void TouchStart(const char*);
private:
	virtual bool init();

	int PressNum=0;
	bool MouseDown=0;
	Point MousePosition;

	void MoveUP(float dt);
	void MoveDOWN(float dt);
	void MoveLEFT(float dt);
	void MoveRIGHT(float dt);

	const SEL_SCHEDULE move[4] = {
	schedule_selector(OPOperator::MoveLEFT),
	schedule_selector(OPOperator::MoveRIGHT),
	schedule_selector(OPOperator::MoveUP),
	schedule_selector(OPOperator::MoveDOWN) };
};
#endif


