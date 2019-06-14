#pragma once
#include"cocos2d.h"
#include"Entity.h"
using namespace cocos2d;
class Weapon;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string , int);
	//CREATE_FUNC(Player);
	static Player* create(std::string , int);
	static Player* create(Entity*);
	bool initWithPlayerType(int id);
	Weapon * ChangeWeapon(int WeaponType);
	Player * getMychara(char* str);
	int CharaType;

	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;
	Sprite *Hand;
	Weapon* weapon;

	bool IsHaveWeapon;
	bool AttackAbleFlag;
	bool AttackEndFlag;

	Vector<SpriteFrame*> MoveFrames[5];

	Vector<SpriteFrame*> AnimationFrames(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end);
	Animate * createAnimate(int FramesIndex, float delay);

	void MoveBegin();

	void MoveEnd();

	void AttackAbleflag(float dt);

	void AttackEndflag(float dt);

	void AttackEnd(int pressnum);

	void AttackBegan(Point MousePosition);

	void AttackMode1(Point TouchPosition);
	void AttackMode2(Point TouchPosition);
	void(Player::*AttackMode)(Point MousePosition);
	float AttackSpeed;
	struct Attr
	{
		uint8_t speed;//速度
		uint8_t attack;//攻击力
		uint8_t hp;//生命值
		uint8_t weapon;//武器
		uint8_t superPower;//加成效果
		uint8_t level;//等级
		uint8_t exp;//经验
	};//角色属性



	int getId();
	std::string getName();
private:
	Entity info;
	std::string m_name;
	int m_id;
	Attr attr;
};
