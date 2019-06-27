#ifndef  _AI_PLAYER_H_
#define  _AI_PLAYER_H_

#include "cocos2d.h"
#include "Player.h"
#include "GameScene.h"
using namespace cocos2d;

class AiPlayer :public Layer {

public:
	CREATE_FUNC(AiPlayer);
	void AiMove(float dt);
	void AiAttack(float dt);
private:
	const int dir[3][3] = { 5,2,6,1,-1,3,4,0,7 };
	int randcount = 0;
	virtual void onEnter();
	virtual bool init();
};
#endif //end AI_PLAYER
