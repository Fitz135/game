#pragma once
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include"cocos2d.h"
#include "Player.h"

class Weapon;
class GameScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	//static Sprite* getCurrentMap();
	static  TMXTiledMap *  getCurrentMap();
	static GameScene * getCurrentScene();

	virtual void onEnter();
	virtual void onExit();
	void update(float delta);
	void SpawnItems(float dt);

	void AiControl(float);

	void MapMove();
	bool isAccessable(Point Position, int Direction);
	void PickMapItems();
	void IsBulletIntoWall();
	void IsBulletIntoPlayer();

	void IsWeaponIntoPlayer();

	void exitCallback(Ref*ref);
	//Sprite* tileMap;
    TMXTiledMap *tileMap;
	TMXLayer *BG;
	TMXLayer *twall;
	TMXLayer *Meta;
	TMXLayer *Foreground;

	CCArray* MapItems;
	CCArray* Players;
	CCArray*Bullets;
	//CCArray*Weapons;
};
#endif // !_GAME_SCENE_H_
