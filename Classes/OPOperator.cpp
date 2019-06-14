#include"OPOperator.h"
#include"GameScene.h"
#include"Player.h"
#include"Settings.h"

#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2

USING_NS_CC;	

static SEL_SCHEDULE move[4] = {
	schedule_selector(OPOperator::MoveLEFT),
	schedule_selector(OPOperator::MoveRIGHT),
	schedule_selector(OPOperator::MoveUP),
	schedule_selector(OPOperator::MoveDOWN) };

bool OPOperator::init() {
	if (!Layer::init()) {
		return false;
	}
	
	PressNum = 0;
	return true;
}

void OPOperator::KeyStart(char* buffer) {
	if (buffer[0] == KeyPress) {
		int keycode = static_cast<int>(buffer[4]) - 48;
		if (0 <= keycode && 3 >= keycode)
		{
			if (!PressNum)
			{
				auto player = dynamic_cast<Player*>(this->getParent());
				player->MoveBegin();
			}
			PressNum++;
			//auto scene = GameScene::getCurrentMap();
			this->schedule(move[keycode], 1.0f / 60);
		}
	}
	else if (buffer[0] == KeyRelease) {
		int keycode = static_cast<int>(buffer[4]) - 48;
		if (0 <= keycode && 3 >= keycode)
		{
			PressNum--;
			//auto scene = GameScene::getCurrentMap();
			this->unschedule(move[keycode]);
			if (!PressNum)
			{
				auto player = dynamic_cast<Player*>(getParent());
				player->MoveEnd();
			}
		}
	}
}
/*void OPOperator::PassOperatorInfo(float dt)
{
	auto player = dynamic_cast<Player*>(getParent());
	if (MouseDown)
		player->AttackBegan(MousePosition);
	if (!MouseDown&&player->AttackEndFlag)
	{
		player->AttackEnd(PressNum);
		this->unschedule(schedule_selector(OPOperator::PassOperatorInfo));
	}
}
bool OPOperator::onMouseBegan(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	MouseDown = 1;
	this->schedule(schedule_selector(OPOperator::PassOperatorInfo), 1.0f / 60);
	return true;
}
void OPOperator::onMouseMoved(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	return;
}
void OPOperator::onMouseEnded(Touch * ptouch, Event *pevent)
{
	MouseDown = 0;
}
*/



void OPOperator::MoveUP(float dt)
{
	MoveBy* move = MoveBy::create(1 / 60, Vec2(0, 1));
	auto player = dynamic_cast<Player*>(getParent());

	player->Legs->runAction(move);
	player->Body->runAction(move->clone());
	player->Head->runAction(move->clone());
}
void OPOperator::MoveDOWN(float dt)
{

	MoveBy* move = MoveBy::create(1 / 60, Vec2(0, -1));
	auto player = dynamic_cast<Player*>(getParent());

	player->Legs->runAction(move);
	player->Body->runAction(move->clone());
	player->Head->runAction(move->clone());
}
void OPOperator::MoveLEFT(float dt)
{
	auto player = dynamic_cast<Player*>(getParent());
	if (player->AttackEndFlag && !MouseDown)
	{
		player->Legs->setFlippedX(true);
		player->Body->setFlippedX(true);
		player->Head->setFlippedX(true);
	}
	MoveBy* move = MoveBy::create(1 / 60, Vec2(-1, 0));
	player->Legs->runAction(move);
	player->Body->runAction(move->clone());
	player->Head->runAction(move->clone());
}
void OPOperator::MoveRIGHT(float dt)
{
	auto player = dynamic_cast<Player*>(getParent());
	if (player->AttackEndFlag && !MouseDown)
	{
		player->Legs->setFlippedX(false);
		player->Body->setFlippedX(false);
		player->Head->setFlippedX(false);
	}
	MoveBy* move = MoveBy::create(1.0f / 60, Vec2(1, 0));

	player->Legs->runAction(move);
	player->Body->runAction(move->clone());
	player->Head->runAction(move->clone());
}