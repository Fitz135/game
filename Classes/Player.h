#pragma once
#include"cocos2d.h"
using namespace cocos2d;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string name, int id);
	//CREATE_FUNC(Player);
	static Player* create(std::string name, int id);
	bool initWithPlayerType();
	Player * getMychara(char* str);

	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;

	bool AttackAbleFlag;
	bool AttackEndFlag;

	Vector<SpriteFrame*> MoveFrames[3];
	Vector<SpriteFrame*> AttackFrames;

	Vector<SpriteFrame*> AnimationFrames(const char * FrameName, int begin, int end);
	Animate * createMoveAni(int i);
	Animate * createAttackAni();

	void MoveBegin();

	void MoveEnd();

	void AttackAbleflag(float dt);

	void AttackEndflag(float dt);

	void AttackEnd(int pressnum);

	void Attack_Shoot(Point MousePosition);

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
private:
	std::string m_name;
	int m_id;
	Attr attr;
};
