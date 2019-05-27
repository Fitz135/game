#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include"cocos2d.h"
class Player :public cocos2d::Node {
public:
	Player();
	virtual ~Player();
	static Player* create();
	void addPlayer();
	bool init();

	int getId();
	void setId(int);
	int getSpeed();
	void setSpeed(int);

	void moveLeft(float f);
	void moveRight(float f);
	void moveUp(float f);
	void moveDown(float f);
private:
	cocos2d::Sprite* sprite;
	int hp;
	int id;
	int speed;
	int atk;
	int def;
	int level;
	int exp;
	int goal;
	//Weapon weapon;
};
#endif // !_PLAYER_H_

