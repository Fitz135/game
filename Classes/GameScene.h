#pragma once
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include"cocos2d.h"
#include "Player.h"

class Weapon;
class GameScene :public cocos2d::Layer {
public:
	CREATE_FUNC(GameScene);
	static cocos2d::Scene* createScene();
	static  TMXTiledMap *  getCurrentMap();
	static GameScene * getCurrentScene();

	bool isAccessable(Point Position, int Direction);

    TMXTiledMap *tileMap;
	TMXLayer *Meta;

	CCArray* MapItems;
	CCArray*Bullets;
	Sprite* Bulletset;
	Sprite* Itemset;
private:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void update(float delta);

	CCArray* Players;
	void Collision();
	void PickMapItems();
	void MoveMap();
	void MovePlayer();
	void exitCallback(Ref * ref);
	void SpawnItems(float dt);
	void SpawnItems(Vec3 pos);
};
#endif // !_GAME_SCENE_H_
