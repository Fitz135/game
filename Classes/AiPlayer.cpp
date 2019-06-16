#include "AiPlayer.h"

#include <string>
#include <set>
#include <map>
#include <vector>

using namespace cocos2d;

bool AiPlayer::init() {
	Vec2 pos;
	int x, y;
	std::string aiName = "AiNumber:" ;
	Player* aiPlayer=new Player();
	

	if (aiPlayer && aiPlayer->initWithPlayerType(i))
	{
		aiPlayer->autorelease();
		aiPlayer->setZOrder(100);
		aiPlayer->setName(aiName + Num[i]);

		x = tileMap->birthPoint[i][1];
		y = tileMap->birthPoint[i][0];
		pos = tileMap->tileCoordToPosition(cocos2d::Vec2(x, y));
		aiPlayer->setPosition(pos);
		aiPlayer->retain();
		tileMap->addChild(aiPlayer);
	}
	
	return true;
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
}

