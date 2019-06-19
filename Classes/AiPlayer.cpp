#include "AiPlayer.h"
#include"Settings.h"
#include"Weapon.h"

using namespace cocos2d;

bool AiPlayer::init() {
	return true;
}
void AiPlayer::onEnter() {
	Layer::onEnter();
	this->scheduleOnce(schedule_selector(AiPlayer::AiMove), 2.0f / 60);
}
void AiPlayer::AiMove(float dt)
{
	auto ai =dynamic_cast<Player*>(this->getParent());
	auto scene = dynamic_cast<GameScene*>(ai->getParent()->getParent());
	auto player = dynamic_cast<Player*>(ai->getParent()->getChildByName("Player"));

	auto aiPosition = ai->getPosition();
	auto playerPosition = player->getPosition();
	int x, y;
	int moveflag=1,kflag=1;
	srand(int(time(0)) + rand());
	while (true)
	{
		auto weap = dynamic_cast<Weapon*>(ai->weapon);
		if (ai->HP <= 0)
		{
		x = static_cast<bool>(playerPosition.x > aiPosition.x) * 2 - 1;
		y = static_cast<bool>(playerPosition.y > aiPosition.y) * 2 - 1;
		break;
		}
		else if (!ai->weapon&&moveflag)
		{
			moveflag = 0;
			auto mapitems = dynamic_cast<Array*>(scene->MapItems);
			if (mapitems->count())
			{
				auto weaponPosition = dynamic_cast<Sprite*>(mapitems->getObjectAtIndex(0))->getPosition();
				x = static_cast<bool>(weaponPosition.x > aiPosition.x) * 2 - 1;
				y = static_cast<bool>(weaponPosition.y > aiPosition.y) * 2 - 1;
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
			x = static_cast<bool>(playerPosition.x > aiPosition.x) * 2 - 1;
			y = static_cast<bool>(playerPosition.y > aiPosition.y) * 2 - 1;
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
	    auto ai = dynamic_cast<Player*>(this->getParent());
		if (ai->IsHaveWeapon)
		{
			auto map = dynamic_cast<TMXTiledMap*>(ai->getParent());
			auto player = map->getChildByName("Player");

			auto mapPosition = map->getPosition();
			auto aiPosition = ai->getPosition();
			auto playerPosition = player->getPosition();
			auto dir = playerPosition + mapPosition;
			ai->AttackBegan(Vec2(dir.x, dir.y));
		}
		this->scheduleOnce(schedule_selector(AiPlayer::AiAttack), ai->AttackSpeed);
}