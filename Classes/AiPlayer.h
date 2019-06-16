#ifndef  _AI_PLAYER_H_
#define  _AI_PLAYER_H_

#include "cocos2d.h"
#include "Player.h"
#include "MapScene.h"
#include <string>
#include <map>
using namespace cocos2d;
class AiPlayer :public Player {
private:
	std::string AiName;
	 MapScene* tileMap = MapScene::getCurrentScene();
	 int i;
	 char Num[7] = { 2,3,4,5,6,7 };
	 int Dirc[8][2] = { {0,1},{1,0},{0,-1},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1} };

public:

	CREATE_FUNC(AiPlayer);
	bool init();
	std::string findMyWay(char aiNum);
	int getManhattanDis(Vec2& from, Vec2& to);
	std::vector<Vec2>::iterator getSmallestF_Gvalue(const std::map<Vec2, int>&, 
						const std::map<Vec2, int>&, std::vector<Vec2> &myStack);
	std::string sortWay( std::vector<Vec2>&, const Vec2& );

};
#endif //end AI_PLAYER
