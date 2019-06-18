#pragma once
#include"cocos2d.h"
#include"Entity.h"
#include"HP.h"

using namespace cocos2d;
class Weapon;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string , int);

	static Player* create(std::string , int);
	static Player* create(Entity*);
	Hp* HPBar;
	Weapon* weapon;

	int MoveSpeed;
	int WeaponType;
	int HP=100;
	float AttackSpeed;
	bool IsHaveWeapon=0;
	bool AttackAbleFlag=1;
	bool IsAI = 0;

	void update(float dt);
	void MoveBegin();
	void MoveEnd();
	void AttackBegan(Point MousePosition);
	void AttackEnd(int pressnum);
	void AttackMode1(Point TouchPosition);
	void AttackMode2(Point TouchPosition);
	void AttackMode3(Point TouchPosition);
	void Dead(Node * who);

	typedef struct 
	{
		uint8_t speed;//速度
		uint8_t attack;//攻击力
		uint8_t hp;//生命值
		uint8_t weapon;//武器
		uint8_t superPower;//加成效果
		uint8_t level;//等级
		uint8_t exp;//经验
	}Attr;//角色属性

	int getId();
private:
	Entity info;
	std::string m_name;
	int m_id;
	Attr attr;

	bool initWithPlayerType(int id);
	Animate * createAnimate(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end, float delay);
	void ChangeWeapon(int WeaponType);
	void AttackAbleflag(float dt);
	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;
	Sprite *Hand;
	int CharaType;
};
