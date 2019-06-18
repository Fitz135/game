#include "AiPlayer.h"
#include"Settings.h"
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace cocos2d;

inline Player* getMyplayer(char* str) {
	auto map = GameScene::getCurrentMap();
	return dynamic_cast<Player*>(map->getChildByName(str));
}

bool AiPlayer::init() {
	return true;
}
void AiPlayer::onEnter() {
	Layer::onEnter();
	this->scheduleOnce(schedule_selector(AiPlayer::AiMove), 2.0f / 60);
}
void AiPlayer::AiMove(float dt)
{
	auto ai = (Player*)this->getParent();
	auto map = (TMXTiledMap*)ai->getParent();
	auto player = (Player*)map->getChildByName("Player");
	auto scene = (GameScene*)map->getParent();
	auto Meta = map->getLayer("Meta");
	auto aiPosition = ai->getPosition();
	auto playerPosition = player->getPosition();
	int x, y;
	int moveflag=1,kflag=1;
	srand(int(time(0)) + rand());
	while (true)
	{
		auto weap = (Weapon*)ai->weapon;
		if (ai->HP <= 0)
		{
		x = (bool)(playerPosition.x > aiPosition.x) * 2 - 1;
		y = (bool)(playerPosition.y > aiPosition.y) * 2 - 1;
		break;
		}
		else if (!ai->weapon&&moveflag)
		{
			moveflag = 0;
			auto mapitems = (Array*)scene->MapItems;
			if (mapitems->count())
			{
				auto weaponPosition = ((Sprite*)mapitems->getObjectAtIndex(0))->getPosition();
				x = (bool)(weaponPosition.x > aiPosition.x) * 2 - 1;
				y = (bool)(weaponPosition.y > aiPosition.y) * 2 - 1;
				if (!scene->isAccessable(aiPosition + Vec2(x, y)*ai->MoveSpeed, dir[x + 1][y + 1]))
				{
					break;
				}
			}
		}
		else if (playerPosition == aiPosition)
		{
			x = 0;
			y = 0;
			break;
		}
		else if(moveflag)
		{
			x = (bool)(playerPosition.x > aiPosition.x) * 2 - 1;
			y = (bool)(playerPosition.y > aiPosition.y) * 2 - 1;
			if(!scene->isAccessable(aiPosition + Vec2(x, y)*ai->MoveSpeed, dir[x + 1][y + 1]))
			{
				break;
			}
			else
			{
				moveflag = 0;
			}
		}
		else if (randcount >= 30)
		{
			x = rand() % 3 - 1;
			y = rand() % 3 - 1;
			for(int k=1;k<30;k++)
			if (!scene->isAccessable(aiPosition + Vec2(x, y)*ai->MoveSpeed*k, dir[y + 1][x + 1]))
			{
				kflag++;
				continue;
			}
			else
			{
				if(k>10)
				randcount = 0;
				kflag--;
				break;
			}
			break;
		}
		else if(!moveflag)
		{
			while (true)
			{
				srand(int(time(0)) + rand());
				x = rand() % 3 - 1;
				y = rand() % 3 - 1;
				randcount++;
				if (!scene->isAccessable(aiPosition + Vec2(x, y)*ai->MoveSpeed, dir[x + 1][y + 1]))
				{
					break;
				}
			}
			break;
		}
	}
	auto move = MoveBy::create(2.0f / 60*kflag, Vec2(x, y)*ai->MoveSpeed*kflag);
	ai->runAction(move);
	this->scheduleOnce(schedule_selector(AiPlayer::AiMove), 2.0f / 60 * kflag);
}
void AiPlayer::AiAttack(float dt)
{

	    auto ai = (Player*)this->getParent();
		if (ai->IsHaveWeapon)
		{
			auto map = (TMXTiledMap*)ai->getParent();
			auto mapPosition = map->getPosition();
			auto player = map->getChildByName("Player");
			auto aiPosition = ai->getPosition();
			auto playerPosition = player->getPosition();
			auto dir = playerPosition + mapPosition;
			ai->AttackBegan(Vec2(dir.x, dir.y));
		}
		this->scheduleOnce(schedule_selector(AiPlayer::AiAttack), ai->AttackSpeed);
}