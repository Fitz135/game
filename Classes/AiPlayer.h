#ifndef  _AI_PLAYER_H_
#define  _AI_PLAYER_H_

#include "cocos2d.h"
#include "Player.h"
#include "GameScene.h"
#include <string>
#include <map>
using namespace cocos2d;
class AiPlayer :public Layer {
private:
	std::string AiName;
	GameScene* tileMap = GameScene::getCurrentScene();
	 int i;
	 char Num[7] = { 2,3,4,5,6,7 };
	 int Dirc[8][2] = { {1,0},{0,-1},{-1,0},{0,1},{1,-1},{-1,-1},{-1,1},{1,1} };
	 int dir[3][3] = {5,2,6,1,-1,3,4,0,7};
	 int randcount = 0;

public:

	CREATE_FUNC(AiPlayer);
	bool init();
	virtual void onEnter();
	void AiMove(float dt);
	void AiAttack(float dt);
	int MapInfo[40][40];
	int Dir[2];
	//void findMyWay(float dt);
	//int getManhattanDis(Vec2& from, Vec2& to);
//std::vector<Vec2>::iterator getSmallestF_Gvalue(const std::map<Vec2, int>&, 
					//	const std::map<Vec2, int>&, std::vector<Vec2> &myStack);
	//std::string sortWay( std::vector<Vec2>&, const Vec2& );

};
#endif //end AI_PLAYER
