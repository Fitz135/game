#include "AiPlayer.h"
#include"Settings.h"
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace cocos2d;

bool AiPlayer::init() {
	return true;
}
void AiPlayer::onEnter() {
	//Layer::onEnter();
	((Player*)this->getParent())->WeaponType = 0;
	((Player*)this->getParent())->ChangeWeapon(0);
	((Player*)this->getParent())->MoveBegin();
	this->getParent()->schedule(schedule_selector(AiPlayer::AiMove), 2.0f / 60);
	this->getParent()->schedule(schedule_selector(AiPlayer::AiAttack), 1.0f / 4);
}
void AiPlayer::AiMove(float dt)
{
	auto ai = (Player*)this->getParent();
	/*auto move = MoveTo::create(5, Vec2(1230, 50));
	auto move1 = MoveTo::create(5, Vec2(1230, 1230));
	auto move2= MoveTo::create(5, Vec2(50, 1230));
	auto move3= MoveTo::create(5, Vec2(50, 50));
	auto seq = Sequence::create(move, move1, move2, move3, nullptr);
	
	ai->runAction(RepeatForever::create(seq));*/

	auto map = (TMXTiledMap*)ai->getParent();
	auto scene = (GameScene*)map->getParent();
	auto Meta = map->getLayer("Meta");
	auto aiPosition = ai->getPosition();
	int x, y;
	while (1)
	{
		srand(int(time(0)) + rand());
		y = rand() % 3 - 1;
		if (y > 0 && !scene->isAccessable(aiPosition + Vec2(0, ai->MoveSpeed), 3))
		{
			break;
		}
		else if (y < 0 && !scene->isAccessable(aiPosition + Vec2(0, -ai->MoveSpeed), 1))
		{
			break;
		}
		else if (y == 0)
			break;
	}
	while (1)
	{
		srand(int(time(0)) + rand());
		x = rand() % 3 - 1;
		if (x > 0 && !scene->isAccessable(aiPosition + Vec2(ai->MoveSpeed,0), 0))
		{
			break;
		}
		else if (x < 0 && !scene->isAccessable(aiPosition + Vec2(-ai->MoveSpeed,0), 2))
		{
			break;
		}
		else if (x == 0)break;
	}
	auto move = MoveBy::create(2.0f / 60, Vec2(x, y)*ai->MoveSpeed);
	ai->runAction(move);
}
void AiPlayer::AiAttack(float dt)
{
	auto ai = (Player*)this->getParent();
	auto map =(TMXTiledMap*)ai->getParent();
	auto mapPosition = map->getPosition();
	auto player=map->getChildByName("Player");
	if (player)
	{
		auto aiPosition = ai->getPosition();
		auto playerPosition = player->getPosition();
		auto dir = playerPosition + mapPosition;
		ai->AttackBegan(Vec2(dir.x, dir.y));
	}
}
/*int AiPlayer::getManhattanDis(Vec2& from, Vec2& to) {
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

/*std::string AiPlayer::sortWay( std::vector<Vec2>& closed, const Vec2& from) {
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


std::string AiPlayer::findMyWay(char aiNum) {
	Player* enmy = getMychara("Player");
	Player* aiMe = getMychara(const_cast<char*>("AiNumber:") + aiNum);

	Vec2 enmyCoord = tileMap->positionToTileCoord(enmy->getPosition());
	Vec2 aiMeCoord = tileMap->positionToTileCoord(aiMe->getPosition());
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

	while (myClosed.find(enmyCoord) == end) {	
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
			if (tileMap->isInMap(newCoord) && tileMap->isAccessable(newCoord)
				&& myClosed.find(newCoord) == end)
			{
				myStack.push_back(newCoord);
				Gvalue.insert(std::pair<Vec2, int>(newCoord, 1+ curGvalue));
				mDis = getManhattanDis(newCoord, enmyCoord);
				Fvalue.insert(std::pair<Vec2, int>(newCoord, mDis));
			}
			end = myClosed.end();
		}
	}
	return const_cast<char*>("AiNumber:") + aiNum  + sortWay(closedStack, aiMeCoord);
}*/

