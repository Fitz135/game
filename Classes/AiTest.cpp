#include"AiTest.h"
AiTest::AiTest(std::string name, int type) :Player(name, type) {
	isUp = false;
	isRight = false;
	isDown = false;
	isLeft = false;
	//
}

AiTest* AiTest::create(std::string name,int type) {
	auto ai = static_cast<AiTest*>(Player::create(name,type));
	ai->isUp = false; // 
	ai->isRight = false;
	ai->isDown = false;
	ai->isLeft = false;
	//ai->_type = type;
	return ai;
}