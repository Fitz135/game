#include"Operator.h"
#include"GameScene.h"
#include"Settings.h"

#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2

SEL_SCHEDULE move[4] = {
	schedule_selector(Operator::MoveLEFT),
	schedule_selector(Operator::MoveRIGHT),
	schedule_selector(Operator::MoveUP),
	schedule_selector(Operator::MoveDOWN) };

inline Player* getMyplayer(char* str) {
	auto scene = GameScene::getCurrentMap();
	return dynamic_cast<Player*>(scene->getChildByName(str));
}

bool Operator::init()
{
	KeyStart();
	MouseStart();
	PressNum = 0;
	return true;
}
void Operator::KeyStart()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Operator::OnKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Operator::OnKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Operator::MouseStart()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Operator::onMouseBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Operator::onMouseMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Operator::onMouseEnded, this);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Operator::PassOperatorInfo(float dt)
{
	auto Player = getMyplayer("Player");
	if (MouseDown&&Player->AttackAbleFlag&&Player->IsHaveWeapon)
	{
		char buffer[MSGSIZE];
		sprintf(buffer, "%c$%d$%d$%d$", MousePress, local_Id, (int)MousePosition.x, (int)MousePosition.y);
		client->Send(buffer, MSGSIZE);
		Player->AttackBegan(MousePosition);
		if (MousePosition.x < Player->getPositionX())
			Player->setScaleX(-1);
		else
			Player->setScaleX(1);
	}
	if (!MouseDown&&Player->AttackEndFlag)
	{
		char buffer[MSGSIZE];
		sprintf(buffer, "%c$%d$%d", MouseRelease, local_Id, PressNum);
		client->Send(buffer, MSGSIZE);
		Player->AttackEnd(PressNum);
		this->unschedule(schedule_selector(Operator::PassOperatorInfo));
	}
}
bool Operator::onMouseBegan(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	MouseDown = 1;
	this->schedule(schedule_selector(Operator::PassOperatorInfo), 1.0f/60);
	return true;
}
void Operator::onMouseMoved(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	return;
}
void Operator::onMouseEnded(Touch * ptouch, Event *pevent)
{
	MouseDown = 0;
}
void Operator::OnKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	int keycode = (int)keyCode - 26;
	if (0 <= keycode && 3 >= keycode)
	{
		char buffer[MSGSIZE];
		sprintf(buffer, "%c$%d$%d", KeyPress, local_Id, keycode);
		client->Send(buffer, MSGSIZE);
		if (!PressNum)
		{
			auto Player = getMyplayer("Player");
			Player->MoveBegin();
		}
		PressNum++;
		this->schedule(move[keycode], 1.0f / 60);
	}
}

void Operator::OnKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	int keycode = (int)keyCode - 26;
	if (0 <= keycode && 3 >= keycode)
	{
		char buffer[MSGSIZE];
		sprintf(buffer, "%c$%d$%d", KeyRelease, local_Id, keycode);
		client->Send(buffer, MSGSIZE);
		PressNum--;
		this->unschedule(move[keycode]);
		if (!PressNum)
		{
			auto Player = getMyplayer("Player");
			Player->MoveEnd();
		}
	}
}

void Operator::MoveUP(float dt)
{
	MoveBy* move = MoveBy::create(1 / 60, Vec2(0, 1));
	auto Player = getMyplayer("Player");

	Player->runAction(move);
}
void Operator::MoveDOWN(float dt)
{

	MoveBy* move = MoveBy::create(1 / 60, Vec2(0, -1));
	auto Player = getMyplayer("Player");

	Player->runAction(move);

}
void Operator::MoveLEFT(float dt)
{
	auto Player = getMyplayer("Player");
	if (Player->AttackEndFlag && !MouseDown)
		Player->setScaleX(-1);
	MoveBy* move = MoveBy::create(1 / 60, Vec2(-1, 0));
	Player->runAction(move);
}
void Operator::MoveRIGHT(float dt)
{
	auto Player = getMyplayer("Player");
	if (Player->AttackEndFlag && !MouseDown)
		Player->setScaleX(1);
	MoveBy* move = MoveBy::create(1.0f / 60, Vec2(1, 0));
	Player->runAction(move);
}