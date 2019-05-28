#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include"cocos2d.h"
class Player :public cocos2d::Node {
public:
	Player();

    
	virtual ~Player();
    
	static Player* create(const std::string& id, int role);//创建角色
    
    void addPlayer();//加入玩家
    
    enum class Direction: std::int8_t
    {
       LEFT,
       RIGHT,
       UP,
       DOWN
    };//移动方向
    
    struct Attr
    {
        unit8_t speed;//速度
        uint8_t attack;//攻击力
        uint8_t hp;//生命值
        uint8_t weapon;//武器
        uint8_t superPower;//加成效果
        uint8_t level;//等级
        uint8_t exp;//经验
    }//角色属性
    
    
    
	uint8_t getSpeed();
    float getRealSpeed();
	void setSpeed(uint8_t speed);//速度

    uint8_t getDamage();
    void setDamage(uint8_t damage);//伤害
    
    bool isAlive();
    
    uint8_t getWeapon();
    void setWeapon(uint8_t newWeapon);//武器
    
    void attack();//攻击


	void moveLeft(float f);
	void moveRight(float f);
	void moveUp(float f);

	void moveDown(float f);//移动
    
private:
	cocos2d::Sprite* sprite;
    Attr attr;
    time_t direction[4];
    Direction direction;
    std::string rolename;
    
    bool init();
    bool initWithRole(int Role);
    bool initAnimation();

};
#endif // !_PLAYER_H_

