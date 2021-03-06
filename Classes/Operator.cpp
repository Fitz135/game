#include"Operator.h"
#include"GameScene.h"
#include"Settings.h"

#define HEADMOVE 0
#define BODYMOVE 1
#define LEGSMOVE 2

inline Player* getMyplayer(char* str) {
	auto map = GameScene::getCurrentMap();
	return dynamic_cast<Player*>(map->getChildByName(str));
}

bool Operator::init()
{
	KeyStart();
	TouchStart();
	return true;
}
void Operator::KeyStart()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Operator::OnKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Operator::OnKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Operator::TouchStart()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Operator::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Operator::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Operator::onThouchEnded, this);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Operator::PassOperatorInfo(float dt)
{
	auto player = getMyplayer("Player");
	if (MouseDown&&player->AttackAbleFlag&&player->IsHaveWeapon)
	{
		if (!gameMode) {
			char buffer[MSGSIZE];
			sprintf(buffer, "%c$%d$%d$%d$", MousePress, local_Id, (int)MousePosition.x, (int)MousePosition.y);
			client->Send(buffer, MSGSIZE);
		}
		
		player->AttackBegan(MousePosition);
	}
	if (!MouseDown&&player->AttackAbleFlag&&player->IsHaveWeapon)
	{

		if (!gameMode) {
			char buffer[MSGSIZE];
			sprintf(buffer, "%c$%d$%d", MouseRelease, local_Id, PressNum);
			client->Send(buffer, MSGSIZE);
		}
		player->AttackEnd(PressNum);
		this->unschedule(schedule_selector(Operator::PassOperatorInfo));
	}
}
bool Operator::onTouchBegan(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	MouseDown = 1;
	this->schedule(schedule_selector(Operator::PassOperatorInfo), 2.0f/60);
	return true;
}
void Operator::onTouchMoved(Touch * ptouch, Event *pevent)
{
	MousePosition = ptouch->getLocation();
	return;
}
void Operator::onThouchEnded(Touch * ptouch, Event *pevent)
{
	MouseDown = 0;
}
void Operator::OnKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	int keycode = (int)keyCode - 26;
	if (0 <= keycode && 3 >= keycode)
	{
		if (!gameMode) {
			char buffer[MSGSIZE];
			sprintf(buffer, "%c$%d$%d", KeyPress, local_Id, keycode);
			client->Send(buffer, MSGSIZE);
		}
		
		if (!PressNum)
		{
			auto player = getMyplayer("Player");
			player->MoveBegin();
		}
		PressNum++;
		this->schedule(move[keycode], 2.0f / 60);
	}
}

void Operator::OnKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	int keycode =static_cast<int>(keyCode) - 26;
	if (0 <= keycode && 3 >= keycode)
	{
		
		if (!gameMode) {
			char buffer[MSGSIZE];
			sprintf(buffer, "%c$%d$%d", KeyRelease, local_Id, keycode);
			client->Send(buffer, MSGSIZE);
		}
		PressNum--;
		this->unschedule(move[keycode]);
		if (!PressNum)
		{
			auto player = getMyplayer("Player");
			player->MoveEnd();
		}
	}
}

void Operator::MoveUP(float dt)
{
	auto scene = GameScene::getCurrentScene();
	auto player = getMyplayer("Player");
	if (player->HP <= 0 || !scene->isAccessable(player->getPosition() + Vec2(0, player->MoveSpeed), 3))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(0, player->MoveSpeed));
		player->runAction(move);
	}
}
void Operator::MoveDOWN(float dt)
{
	auto scene = GameScene::getCurrentScene();
	auto player = getMyplayer("Player");

	if (player->HP <= 0 || !scene->isAccessable(player->getPosition() + Vec2(0, -player->MoveSpeed), 1))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(0, -player->MoveSpeed));
		player->runAction(move);
	}

}
void Operator::MoveLEFT(float dt)
{
	auto scene = GameScene::getCurrentScene();
	auto player = getMyplayer("Player");
	if (player->WeaponType==5||(player->AttackAbleFlag && !MouseDown))
		player->setScaleX(-1);
	if (player->HP <= 0 || !scene->isAccessable(player->getPosition() + Vec2(-player->MoveSpeed, 0), 2))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(-player->MoveSpeed, 0));
		player->runAction(move);
	}
}
void Operator::MoveRIGHT(float dt)
{
	auto scene = GameScene::getCurrentScene();
	auto player = getMyplayer("Player");
	if (player->WeaponType == 5 || (player->AttackAbleFlag && !MouseDown))
		player->setScaleX(1);
	if (player->HP<=0||!scene->isAccessable(player->getPosition() + Vec2(player->MoveSpeed, 0), 0))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(player->MoveSpeed, 0));
		player->runAction(move);
	}
}