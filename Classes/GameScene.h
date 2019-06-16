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
	void MapMove();
	bool isAccessable(Point Position, int Direction);
	void PickMapItems();
	void IsBulletIntoWall();
	void IsBulletIntoPlayer();


	////////////ai/////////////
	bool isInMap(const cocos2d::Vec2& pos);
	Vec2 positionToTileCoord(const cocos2d::Vec2 & pos);
	int birthPoint[7][2];
	Vec2 tileCoordToPosition(const cocos2d::Vec2 & coord);
	////////////ai/////////////





	//void IsWeaponIntoPlayer();
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
