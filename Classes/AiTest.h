#pragma once
#ifndef _AITEST_H_
#define _AITEST_H_

#include "cocos2d.h"
#include "Player.h"

class AiTest :public Player {
public:
	bool isUp;
	bool isDown;
	bool isRight;
	bool isLeft;
	bool isAtk;
	int _type;
	static AiTest* create(std::string,int );

	AiTest(std::string,int);
};

#endif // !_AITEST_H
