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

void OPOperator::KeyStart(const char* buffer) {
	if (buffer[0] == KeyPress) {
		int keycode = static_cast<int>(buffer[4]) - 48;
		
			if (!PressNum)
			{
				auto player = dynamic_cast<Player*>(this->getParent());
				player->MoveBegin();
			}
			PressNum++;
			//auto scene = GameScene::getCurrentMap();
			this->schedule(move[keycode], 2.0f / 60);
		
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
void OPOperator::MouseStart(const char* buffer) {
	auto player = dynamic_cast<Player*>(getParent());
	if (buffer[0] == MousePress) {
		int x=0, y=0;
		char c_x[5];
		char c_y[5];
		int start_pos = 4;
		int tps = 4;
		for (int j = start_pos; j < MSGSIZE; j++) {
			if (buffer[j] == '$') {
				start_pos = j;
				break;
			}
		}
		strncpy(c_x, &buffer[tps], start_pos - tps);
		c_x[start_pos - tps] = '\0';
		for (int i = 0; i < start_pos - tps; i++) {
			int q = 1;
			for (int k = 1; k < start_pos - tps - i; k++) {
				q *= 10;
			}
			x += (static_cast<int>(buffer[tps+i]) - 48)*q;
		}
		tps = start_pos + 1;
		for (int j = start_pos+1; j < MSGSIZE; j++) {
			if (buffer[j] == '$') {
				start_pos = j;
				break;
			}
		}
		strncpy(c_y, &buffer[tps], start_pos - tps);
		c_y[start_pos - tps] = '\0';
		for (int i = 0; i < start_pos - tps; i++) {
			int q = 1;
			for (int k = 1; k < start_pos - tps - i; k++) {
				q *= 10;
			}
			y += (static_cast<int>(buffer[tps+i]) - 48)*q;
		}
		
	
		player->AttackBegan(Vec2(x,y));
	}
	if (buffer[0] == MouseRelease) {
		int pressnum = 0;
		pressnum = static_cast<int>(buffer[4]) - 48;
		player->AttackEnd(pressnum);
	}
}
/*void OPOperator::PassOperatorInfo(float dt)
{
	auto player = dynamic_cast<Player*>(getParent());
	if (MouseDown&&Player->AttackAbleFlag&&Player->IsHaveWeapon)
	{
		Player->AttackBegan(MousePosition);
		if (MousePosition.x < Player->getPositionX())
			Player->setScaleX(-1);
		else
			Player->setScaleX(1);
	}
	if (!MouseDown&&Player->AttackEndFlag)
	{
		Player->AttackEnd(PressNum);
		this->unschedule(schedule_selector(Operator::PassOperatorInfo));
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
	auto scene = ((GameScene*)this->getParent()->getParent()->getParent());
	auto player = dynamic_cast<Player*>(getParent());
	if (!scene->isAccessable(player->getPosition() + Vec2(0, player->MoveSpeed), 3))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(0, player->MoveSpeed));
		player->runAction(move);
	}
}
void OPOperator::MoveDOWN(float dt)
{
	auto scene = ((GameScene*)this->getParent()->getParent()->getParent());
	auto player = dynamic_cast<Player*>(getParent());

	if (!scene->isAccessable(player->getPosition() + Vec2(0, -player->MoveSpeed), 1))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(0, -player->MoveSpeed));
		player->runAction(move);
	}

}
void OPOperator::MoveLEFT(float dt)
{
	auto scene = ((GameScene*)this->getParent()->getParent()->getParent());
	auto player = dynamic_cast<Player*>(getParent());
	if (player->AttackEndFlag && !MouseDown)
		player->setScaleX(-1);
	if (!scene->isAccessable(player->getPosition() + Vec2(-player->MoveSpeed, 0), 2))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(-player->MoveSpeed, 0));
		player->runAction(move);
	}
}
void OPOperator::MoveRIGHT(float dt)
{
	auto scene = ((GameScene*)this->getParent()->getParent()->getParent());
	auto player = dynamic_cast<Player*>(getParent());
	if (player->AttackEndFlag && !MouseDown)
		player->setScaleX(1);
	if (!scene->isAccessable(player->getPosition() + Vec2(player->MoveSpeed, 0), 0))
	{
		MoveBy* move = MoveBy::create(2.0f / 60, Vec2(player->MoveSpeed, 0));
		player->runAction(move);
	}
}