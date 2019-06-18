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
int AiPlayer::getManhattanDis(Vec2& from, Vec2& to) {
	return abs(from.x - to.x) + abs(from.y + to.y);
}

std::vector<Vec2>::iterator AiPlayer::getSmallestF_Gvalue(const std::map<Vec2, int>& F, const std::map<Vec2, int>& G,
			std::vector<Vec2> &myStack) 
{
	std::vector<Vec2>::iterator begin, end, target;
	begin = myStack.begin();
	end = myStack.end();
	int min = 999999;
	int cur = 0;
	while (begin != end) {
		cur = F.count(*begin) + G.count(*begin);
		if (cur < min)
		{
			min = cur;
			target = begin;
		}
	}
	return target;
}

std::string AiPlayer::sortWay( std::vector<Vec2>& closed, const Vec2& from) {
	std::string rout;
	std::vector<Vec2>::iterator begin, end;
	begin = closed.begin();
	end = closed.end();
	end--;
	while (end != begin)
	{
		rout += end->x + '$' + end->y;
		end--;
	}
	rout += end->x + '$' + end->y;
	return rout;
}


void AiPlayer::findMyWay(float dt) {
	
	auto aiMe =dynamic_cast<Player*>(this->getParent());
	auto scene = dynamic_cast<GameScene*>(aiMe->getParent()->getParent());
	if (aiMe)
	{
		Player* player = dynamic_cast<Player*>(aiMe->getParent()->getChildByName("Player"));

		Vec2 playerCoord = scene->positionToTileCoord(player->getPosition());
		Vec2 aiMeCoord = scene->positionToTileCoord(aiMe->getPosition());
		Vec2 curCoord, newCoord;

		std::vector<Vec2> myStack, closedStack;
		std::map<Vec2, int> Gvalue, Fvalue;
		std::set<Vec2> myClosed;
		std::set<Vec2>::iterator end;
		end = myClosed.end();
		int mDis, curGvalue;
		std::vector <Vec2>::iterator target;

		myStack.push_back(aiMeCoord); //将当前位置压入搜索栈
		myClosed.insert(aiMeCoord);   //将当前位置标记为closed
		Gvalue.insert(std::pair<Vec2, int>(aiMeCoord, 0));

		while (myClosed.find(playerCoord) == end) {
			target = getSmallestF_Gvalue(Gvalue, Fvalue, myStack);
			curCoord = *target;
			myStack.erase(target);
			curGvalue = Gvalue.count(curCoord);
			myClosed.insert(curCoord);
			closedStack.push_back(curCoord);

			for (int i = 0; i < 8; i++)
			{
				newCoord.x = curCoord.x + Dirc[i][0];
				newCoord.y = curCoord.y + Dirc[i][1];
				if (scene->isInMap(newCoord) && scene->isAccessable(newCoord, i)
					&& myClosed.find(newCoord) == end)
				{
					myStack.push_back(newCoord);
					Gvalue.insert(std::pair<Vec2, int>(newCoord, 1 + curGvalue));
					mDis = getManhattanDis(newCoord, playerCoord);
					Fvalue.insert(std::pair<Vec2, int>(newCoord, mDis));
				}
				end = myClosed.end();
			}
		}
		std::vector<Vec2>::iterator finaend = closedStack.end();
		finaend--;
		auto nextPosition = Vec2(finaend->x, finaend->y);
		log("%d %d", nextPosition.x, nextPosition.y);
		auto move = MoveTo::create(2.0f / 60, nextPosition);
		aiMe->runAction(move);
	}
	return;// const_cast<char*>("AiNumber:") + sortWay(closedStack, aiMeCoord);
}

